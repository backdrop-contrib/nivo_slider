<?php
/**
 * @file
 * Generate configuration form and save settings.
 */

/**
 * Implements hook_form().
 */
function nivo_slider_option_configuration_form($form, &$form_state) {
  // Create a list of the currently available themes
  $themes = module_invoke_all('nivo_slider_theme');
  $available_themes = array();

  foreach ($themes as $theme => $properties) {
    $available_themes[$theme] = $properties['name'];
  }

  // Theme
  $form['nivo_slider_theme'] = array(
    '#type' => 'select',
    '#title' => t('Styles'),
    '#options' => $available_themes,
    '#default_value' => variable_get('nivo_slider_theme', 'default'),
    '#description' => t('Select a slider theme. The theme determines the overall appearance of the slider.'),
  );
  // General
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['general']['nivo_slider_random_start'] = array(
    '#type' => 'radios',
    '#title' => t('Random start'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_random_start', 'true'),
    '#description' => t('Start the slideshow with a random slide.'),
  );
  $form['general']['nivo_slider_start_slide'] = array(
    '#type' => 'textfield',
    '#title' => t('Starting slide'),
    '#default_value' => variable_get('nivo_slider_start_slide', '0'),
    '#description' => t('Enter the number of the slide to begin the slideshow with. Slides begin at 0.'),
    '#states' => array(
      'visible' => array(
        ':input[name="nivo_slider_random_start"]' => array('value' => 'false'),
      ),
    ),
  );
  $form['general']['nivo_slider_caption_opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Caption opacity'),
    '#default_value' => variable_get('nivo_slider_caption_opacity', '0.8'),
    '#description' => t('Enter a number between 0 and 1. The opacity determines the transparency of slide captions, where 0 is fully transparent and 1 is fully opaque.'),
  );
  // Effects
  $form['effects'] = array(
    '#type' => 'fieldset',
    '#title' => t('Effects'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['effects']['nivo_slider_effect'] = array(
    '#type' => 'select',
    '#title' => t('Effect'),
    '#options' => array(
      'sliceDown' => t('Slice Down'),
      'sliceDownLeft' => t('Slice Down Left'),
      'sliceUp' => t('Slice Up'),
      'sliceUpLeft' => t('Slice Up Left'),
      'sliceUpDown' => t('Slice Up Down'),
      'sliceUpDownLeft' => t('Slice Up Down Left'),
      'fold' => t('Fold'),
      'fade' => t('Fade'),
      'random' => t('Random'),
      'slideInRight' => t('Slide In Right'),
      'slideInLeft' => t('Slide in Left'),
      'boxRandom' => t('Box Random'),
      'boxRain' => t('Box Rain'),
      'boxRainReverse' => t('Box Rain Reverse'),
      'boxRainGrow' => t('Box Rain Grow'),
      'boxRainGrowReverse' => t('Box Rain Grow Reverse'),
    ),
    '#default_value' => variable_get('nivo_slider_effect', 'random'),
    '#description' => t('Select an effect. The chosen effect will be used to transition between slides.'),
   );
  $form['effects']['effect_properties'] = array(
    '#type' => 'fieldset',
    '#title' => t('Effect properties'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['effects']['effect_properties']['nivo_slider_slices'] = array(
    '#type' => 'textfield',
    '#title' => t('Slices'),
    '#default_value' => variable_get('nivo_slider_slices', '15'),
    '#description' => t('Used for slice animations.'),
  );
  $form['effects']['effect_properties']['nivo_slider_box_columns'] = array(
    '#type' => 'textfield',
    '#title' => t('Box columns'),
    '#default_value' => variable_get('nivo_slider_box_columns', '8'),
    '#description' => t('Used for box animations.'),
  );
  $form['effects']['effect_properties']['nivo_slider_box_rows'] = array(
    '#type' => 'textfield',
    '#title' => t('Box rows'),
    '#default_value' => variable_get('nivo_slider_box_rows', '4'),
    '#description' => t('Used for box animations.'),
  );
  $form['effects']['nivo_slider_animation_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation speed'),
    '#default_value' => variable_get('nivo_slider_animation_speed', '500'),
    '#description' => t('Enter a time in milliseconds. Determines how long it takes to transition from one slide to the next.'),
  );
  $form['effects']['nivo_slider_pause_on_hover'] = array(
    '#type' => 'radios',
    '#title' => t('Pause animation when hovering over the slide'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_pause_on_hover', 'true'),
  );
  // Navigation
  $form['navigation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navigation'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['navigation']['nivo_slider_directional_navigation'] = array(
    '#type' => 'radios',
    '#title' => t('Directional navigation'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_directional_navigation', 'true'),
    '#description' => t('Display next and previous navigation buttons.'),
  );
  $form['navigation']['nivo_slider_next_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Next button text'),
    '#default_value' => variable_get('nivo_slider_next_text', t('Next')),
    '#states' => array(
      'visible' => array(
        ':input[name="nivo_slider_directional_navigation"]' => array('value' => 'true'),
      ),
    ),
  );
  $form['navigation']['nivo_slider_previous_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous button text'),
    '#default_value' => variable_get('nivo_slider_previous_text', t('Prev')),
    '#states' => array(
      'visible' => array(
        ':input[name="nivo_slider_directional_navigation"]' => array('value' => 'true'),
      ),
    ),
  );
  $form['navigation']['nivo_slider_directional_navigation_hide'] = array(
    '#type' => 'radios',
    '#title' => t('Directional navigation hide'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_directional_navigation_hide', 'true'),
    '#description' => t('Only show on hover.'),
    '#states' => array(
      'visible' => array(
        ':input[name="nivo_slider_directional_navigation"]' => array('value' => 'true'),
      ),
    ),
  );
  $form['navigation']['nivo_slider_control_navigation'] = array(
    '#type' => 'radios',
    '#title' => t('Control navigation'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_control_navigation', 'true'),
    '#description' => t('Display individual slide number navigation buttons.'),
  );
  $form['navigation']['nivo_slider_control_nav_thumbs'] = array(
    '#type' => 'radios',
    '#title' => t('Control navigation thumbnails'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_control_nav_thumbs', 'false'),
    '#description' => t('Use thumbnails for Control Nav.'),
    '#states' => array(
      'visible' => array(
        ':input[name="nivo_slider_control_navigation"]' => array('value' => 'true'),
      ),
    ),
  );
  $form['navigation']['nivo_slider_keyboard_navigation'] = array(
    '#type' => 'radios',
    '#title' => t('Keyboard navigation'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_keyboard_navigation', 'true'),
    '#description' => t('Allow slideshow navigation using the left and right keyboard arrow keys.'),
  );
  $form['navigation']['nivo_slider_manual_advance'] = array(
    '#type' => 'radios',
    '#title' => t('Manual advance'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_manual_advance', 'false'),
    '#description' => t('Automatic slide transitions.'),
  );
  $form['navigation']['nivo_slider_pause_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause time'),
    '#default_value' => variable_get('nivo_slider_pause_time', '3000'),
    '#description' => t('Enter a time in milliseconds. Determines how long each slide is displayed before transitioning to the next slide.'),
    '#states' => array(
      'visible' => array(
        ':input[name="nivo_slider_manual_advance"]' => array('value' => 'false'),
      ),
    ),
  );

  return system_settings_form($form);
}
