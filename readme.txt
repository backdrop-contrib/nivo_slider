CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Permissions
 * Usage
 * Sponsors

INTRODUCTION
------------

Current Maintainers:

 * Devin Carlson <http://drupal.org/user/290182>

Nivo Slider provides an easy and eye-catching way to showcase featured content.

Nivo Slider gives administrators four important tools:

 * A simple method of adding slides to the slideshow.
 * An administration interface to configure slideshow settings.
 * Simple slider positioning using the Drupal block system.
 * Integration with the Drupal image system to automatically crop, scale and
   style slide images.

REQUIREMENTS
------------

Nivo Slider has eight dependencies, most of which are related to the image
module.

Drupal core modules
 * Block
 * Field
 * Field SQL Storage
 * File
 * Image
 * Menu
 * Options

Contributed modules
 * Libraries API

INSTALLATION
------------

To install Nivo Slider:

 * Copy the Libraries API module to sites/all/modules or sites/sitename/modules
   as you require.
 * Download the latest version of the Nivo Slider jQuery plugin
   (http://nivo.dev7studios.com/pricing/) and extract it to
   sites/all/libraries or sites/sitename/libraries as you require. The
   extracted folder must be named nivo-slider.
 * Enable Nivo Slider and all of the modules that it requires.

PERMISSIONS
------------

Once a user role has access to sub or sibling menus, Submenu Tree becomes 
immediately useful.

Submenu Tree provides three permissions:

 * Administer submenu tree
    Allows a user to configure the display of sub or sibling content and the 
    default options when editing content.
 * Administer submenus
    Allows a user to edit sub content options when editing content.
 * Administer Siblingmenus
    Allows a user to edit sibling content options when editing content.

USAGE
-----

Nivo Slider provides a block which will determine where the slider is placed.

Administrators or users with the Administer Nivo Slider permission will be able
to configure slider settings by visiting the Structure administration page. 

Nivo Slider provides two configuration pages: slides and options.
 * Slides
    The slides page provides an interface for managing the content of the
    slider. Slider content can be given a title or description, made into a
    link, reordered or restricted to displaying on a specific path.
 * Options
    The options page provides an interface for configuring the slider. Slider
    options include general options, transitions and navigation. 

Sponsors
--------

Development of Nivo Slider is sponsored by the Ontario Ministry of Northern
Development and Mines (http://www.mndm.gov.on.ca).

Thanks also goes out to the Marinelli theme
(http://drupal.org/project/marinelli) and its developer(s) for providing the
base code which powers the banner administration and generation.