<?php
/**
 * @file
 * Hooks provided by the Nivo Slider module.
 *
 * Modules and themes may implement any of the available hooks to interact with
 * the slider.
 */

/**
 * Register a custom slider theme.
 *
 * This hook can be used to register a custom theme for the slider. Custom
 * themes will be displayed and made selectable on the slider options
 * administration page.
 *
 * Custom slider themes get a unique CSS class to use for styling and can
 * specify an unlimited number of CSS and JS files to include when the slider
 * is displayed. Themes may also choose to override the default CSS and JS
 * added by the Nivo Slider jQuery plugin. This is useful when heavy
 * customization of the slider is required.
 */
function hook_nivo_slider_theme() {
  return array(
    'theme_name' => array(
      'name' => t('Theme name'), // Human readable theme name
      'description' => t('Theme description.'), // Description of the theme
      'override' => array(
        'css' => FALSE, // Override the default Nivo Slider jQuery plugin cascading style sheet
        'js' => FALSE, // Override the default Nivo Slider jQuery plugin javascript
      ),
      'resources' => array(
        'css' => array(
          drupal_get_path('module', 'module_name') . '/css/example.css', // Full path to a CSS file to include with the theme
          drupal_get_path('module', 'module_name') . '/css/demonstration.css',
        ),
        'js' => array(
          drupal_get_path('module', 'module_name') . '/js/example.css', // Full path to a JS file to include with the theme
          drupal_get_path('module', 'module_name') . '/js/demonstration.css',
        ),
      ),
    )
  );
}
