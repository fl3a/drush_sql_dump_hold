# drush_sql_dump_hold

## Introduction

This [Drush](http://drupal.org/project/drush) extension adds a hold option and argument (e.g. `--hold=31`)
to drush's core sql-dump command,
which lets you hold specified amount of sql dumps,
before purging oldest above e.g. 31 in path from --target-file (extracted path or pwd)

**Perhaps useful for periodic execution of drush sql-dump?!**

## Installation

### Manual method

Download and extract this extension to a number of places:

- In a .drush folder in your HOME folder.
- Along with one of your existing modules. If your command is related to an existing module, this is the preferred option.
- In a folder specified with the include option.
- In /path/to/drush/commands (not a "Smart Thing to Do", but it would work).

### Drush method 

Just install it via drush, type:
	
	drush dl drush_sql_dump_hold

## Implementation


This Drush extension just implemented `drush_hook_post_COMMAND()`, 
in this case, it is `drush_sql_dump_hold_post_sql_dump_execute()`,
which is fired after the execution of `drush_sql_dump_execute()`.

This extension has also implemented `hook_drush_help_alter()`,
to add the hold option to Drush's `sql-dump` command.

## Documentation

- [Blog post about drush_sql_dump](https://reinblau.de/de/news/entwicklung-neue-option-fuer-drush-sql-dump) (in german)
- [example.aliases.drushrc.php](http://drupalcode.org/project/drush_sql_dump_hold.git/blob_plain/HEAD:/example.aliases.drushrc.php) for options
