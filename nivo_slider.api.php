<?php
/**
 * @file
 * Hook to register a Nivo Slider theme.
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
