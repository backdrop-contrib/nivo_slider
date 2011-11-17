<?php
/**
 * Hook to register a Nivo Slider theme.
 */
function hook_nivo_slider_style() {
  return array(
    'theme' => array(
      'name' => 'style_name', // Human readable style name
      'description' => t('Theme description.'), // Description of the style
      'path' => array(
        'css' => drupal_get_path('module', 'my_module') . '/css/', // The path (including a trailing slash) to the theme's stylesheet location
        'js' => drupal_get_path('module', 'my_module') . '/js/', // The path (including a trailing slash) to the theme's javascript location
      ),
    )
  );
}
