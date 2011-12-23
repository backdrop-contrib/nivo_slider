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
    '#title' => t('Theme'),
    '#options' => $available_themes,
    '#default_value' => variable_get('nivo_slider_theme', 'default'),
    '#description' => t('Select a slider theme. The slider theme determines the overall appearance of the slider.'),
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
    '#title' => t('Use a random starting slide'),
    '#options' => array(
      'true' => t('Yes'),
      'false' => t('No'),
    ),
    '#default_value' => variable_get('nivo_slider_random_start', 'true'),
    '#description' => t('Randomly select a slide to begin the slideshow with.'),
  );
  $form['general']['nivo_slider_start_slide'] = array(
    '#type' => 'textfield',
    '#title' => t('Starting slide'),
    '#maxlength' => 3,
    '#size' => 3,
    '#default_value' => variable_get('nivo_slider_start_slide', '0'),
    '#description' => t('Specify which slide the slideshow begins with.'),
    '#field_prefix' => t('Slide #'),
    '#states' => array(
      'visible' => array(
        ':input[name="nivo_slider_random_start"]' => array('value' => 'false'),
      ),
    ),
  );
  $form['general']['nivo_slider_caption_opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Caption opacity'),
    '#maxlength' => 3,
    '#size' => 3,
    '#default_value' => variable_get('nivo_slider_caption_opacity', '0.8'),
    '#description' => t('Specify the transparency of slide captions. Enter a number between 0 and 1 where 0 is fully transparent and 1 is fully opaque.'),
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
    '#maxlength' => 3,
    '#size' => 3,
    '#default_value' => variable_get('nivo_slider_slices', '15'),
    '#description' => t('Used for slice animations.'),
    '#field_suffix' => t('Slices'),
  );
  $form['effects']['effect_properties']['nivo_slider_box_columns'] = array(
    '#type' => 'textfield',
    '#title' => t('Box columns'),
    '#maxlength' => 3,
    '#size' => 3,
    '#default_value' => variable_get('nivo_slider_box_columns', '8'),
    '#description' => t('Used for box animations.'),
    '#field_suffix' => t('Columns'),
  );
  $form['effects']['effect_properties']['nivo_slider_box_rows'] = array(
    '#type' => 'textfield',
    '#title' => t('Box rows'),
    '#maxlength' => 3,
    '#size' => 3,
    '#default_value' => variable_get('nivo_slider_box_rows', '4'),
    '#description' => t('Used for box animations.'),
    '#field_suffix' => t('Rows'),
  );
  $form['effects']['nivo_slider_animation_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation speed'),
    '#maxlength' => 6,
    '#size' => 6,
    '#default_value' => variable_get('nivo_slider_animation_speed', '500'),
    '#description' => t('Enter a time in milliseconds. The animation speed determines how long it takes to transition from one slide to the next.'),
    '#field_suffix' => t('ms'),
  );
  $form['effects']['nivo_slider_pause_on_hover'] = array(
    '#type' => 'radios',
    '#title' => t('Pause animation when hovering over the slide'),
    '#options' => array(
      'true' => t('Yes'),
      'false' => t('No'),
    ),
    '#default_value' => variable_get('nivo_slider_pause_on_hover', 'true'),
    '#description' => t('Disable slide transitions while the user is hovering over the slide.'),
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
    '#title' => t('Display directional navigation'),
    '#options' => array(
      'true' => t('Yes'),
      'false' => t('No'),
    ),
    '#default_value' => variable_get('nivo_slider_directional_navigation', 'true'),
    '#description' => t('Display navigation buttons to go back to the previous slide and move forward to the next slide.'),
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
    '#title' => t('Only display directional navigation on hover'),
    '#options' => array(
      'true' => t('Yes'),
      'false' => t('No'),
    ),
    '#default_value' => variable_get('nivo_slider_directional_navigation_hide', 'true'),
    '#description' => t('Hide the directional navigation buttons until the user hovers over the slide.'),
    '#states' => array(
      'visible' => array(
        ':input[name="nivo_slider_directional_navigation"]' => array('value' => 'true'),
      ),
    ),
  );
  $form['navigation']['nivo_slider_control_navigation'] = array(
    '#type' => 'radios',
    '#title' => t('Display control navigation'),
    '#options' => array(
      'true' => t('Yes'),
      'false' => t('No'),
    ),
    '#default_value' => variable_get('nivo_slider_control_navigation', 'true'),
    '#description' => t('Display navigation buttons to select an individual slide.'),
  );
  $form['navigation']['nivo_slider_control_nav_thumbs'] = array(
    '#type' => 'radios',
    '#title' => t('Use thumbnails for control navigation'),
    '#options' => array(
      'true' => t('Yes'),
      'false' => t('No'),
    ),
    '#default_value' => variable_get('nivo_slider_control_nav_thumbs', 'false'),
    '#description' => t('Use thumbnails of each slide as control navigation buttons.'),
    '#states' => array(
      'visible' => array(
        ':input[name="nivo_slider_control_navigation"]' => array('value' => 'true'),
      ),
    ),
  );
  $form['navigation']['nivo_slider_keyboard_navigation'] = array(
    '#type' => 'radios',
    '#title' => t('Enable keyboard navigation'),
    '#options' => array(
      'true' => t('Yes'),
      'false' => t('No'),
    ),
    '#default_value' => variable_get('nivo_slider_keyboard_navigation', 'true'),
    '#description' => t('Allow slideshow navigation using the left and right keyboard arrow keys.'),
  );
  $form['navigation']['nivo_slider_manual_advance'] = array(
    '#type' => 'radios',
    '#title' => t('Disable automatic slide transitions'),
    '#options' => array(
      'true' => t('Yes'),
      'false' => t('No'),
    ),
    '#default_value' => variable_get('nivo_slider_manual_advance', 'false'),
    '#description' => t('For the use to manually navigate between slides.'),
  );
  $form['navigation']['nivo_slider_pause_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause time'),
    '#maxlength' => 6,
    '#size' => 6,
    '#default_value' => variable_get('nivo_slider_pause_time', '3000'),
    '#description' => t('Enter a time in milliseconds. The pause time determines how long each slide is displayed before transitioning to the next slide.'),
    '#field_suffix' => t('ms'),
    '#states' => array(
      'visible' => array(
        ':input[name="nivo_slider_manual_advance"]' => array('value' => 'false'),
      ),
    ),
  );

  return system_settings_form($form);
}

function nivo_slider_option_configuration_form_validate($form, &$form_state) {
  // Opacity must be between 0 and 1
  if ($form_state['values']['nivo_slider_caption_opacity'] < 0 || $form_state['values']['nivo_slider_caption_opacity'] > 1) {
    form_set_error('nivo_slider_caption_opacity', t('Caption opacity must be between 0 and 1.'));
  }
}
