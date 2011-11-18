<?php
/**
 * Hook to register a Nivo Slider theme.
 */
function hook_nivo_slider_theme() {
  return array(
    'theme' => array(
      'name' => t('Theme name'), // Human readable theme name
      'description' => t('Theme description.'), // Description of the theme
      'path' => array(
        'css' => drupal_get_path('module', 'my_module') . '/css/', // The path (including a trailing slash) to the theme's stylesheet location
        'js' => drupal_get_path('module', 'my_module') . '/js/', // The path (including a trailing slash) to the theme's javascript location
      ),
    )
  );
}
