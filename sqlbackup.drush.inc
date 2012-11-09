<?php

/**
 * @file Drush sql-dump extension
 */


/**
 * Implements hook_drush_help_alter().
 *
 * Adds --hold option to drush's sql-dump command
 */
function sqlbackup_drush_help_alter(&$command) {
  if ($command['command'] == 'sql-dump') {
    $command['options']['hold'] = 'Hold the number of dumps, before purging oldest.';
  }
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * Hold the number of dumps, before purging oldest,
 * if hold and result-file option were provided
 */
function drush_sqlbackup_post_sql_dump_execute($db_spec = NULL) {
  $file = drush_get_option('result-file', FALSE);
  $hold = drush_get_option('hold', FALSE);

  // no drush_hook_COMMAND_validate() here, 
  // we want a proper execution of sql-dump and skip only purging/drush_hook_post_COMMAND()
  if ($hold && !is_numeric($hold)) {
    return drush_log('Hold option is not numeric.', 'error');  
  }

  if ($file && $hold) {
    $path = explode('/', realpath($file));
    unset($path[count($path) - 1]);
    $path = implode('/', $path);
    if (is_dir($path)) {
      if($dh = opendir($path)) {
        while ($filename = readdir($dh)) {
          $file = $path . '/' .  $filename;
          $path_parts = pathinfo($file);
          if ((is_file($file)) && ($path_parts['extension'] == 'sql' || $path_parts['extension'] == 'gz')) {
            // use created time for order...
            $ctime = filectime($file); 
            $files[$ctime] = $file;
          }
        }
        // sort and reverse order
        ksort($files);
        $files = array_reverse($files);

        // slice array at $hold to get items to purge
        $purge = array_slice($files, $hold);

        if (count($purge)) {        
          if (drush_confirm(dt("Do you really want to purge the following dumps:\n!dumps", 
              array("!dumps" => implode("\n", $purge))))) 
          {
            foreach ($purge as $file) {
              unlink($file);
            }	
            drush_log(dt("Purged dumps:\n!dumps", array('!dumps' => implode("\n", $purge))), 'success');
          }
          else {
            drush_print(dt('Aborting.'));
          }
        }
        else {
          drush_print(dt('No files to purge'));
        }
      }
    }
  }
}
