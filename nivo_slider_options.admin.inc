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

  foreach ($themes as $i => $theme) {
    if (file_exists($theme['path']['css'] . $i . '.css')) {
      $available_themes[$i] = $theme['name'];
    }
  }

  // Theme
  $form['nivo_slider_theme'] = array(
    '#type' => 'select',
    '#title' => t('Styles'),
    '#options' => $available_themes,
    '#default_value' => variable_get('nivo_slider_theme', 'default'),
    '#description' => t('Available themes.'),
  );

  // Options
  $form['nivo_slider_start_slide'] = array(
    '#type' => 'textfield',
    '#title' => t('Start slide'),
    '#default_value' => variable_get('nivo_slider_start_slide', '0'),
    '#description' => t(''),
  );
  $form['nivo_slider_random_start'] = array(
    '#type' => 'radios',
    '#title' => t('Random start'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_random_start', 'true'),
    '#description' => t('Start on a random slide.'),
  );
  $form['nivo_slider_caption_opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Opacity'),
    '#default_value' => variable_get('nivo_slider_caption_opacity', '0.8'),
    '#description' => t('Universal caption opacity.'),
  );
  $form['effect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Effect'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['effect']['nivo_slider_effect'] = array(
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
    '#description' => t('Slider animation.'),
   );
  $form['effect']['nivo_slider_slices'] = array(
    '#type' => 'textfield',
    '#title' => t('Slices'),
    '#default_value' => variable_get('nivo_slider_slices', '15'),
    '#description' => t('For slice animations.'),
  );
  $form['effect']['nivo_slider_box_columns'] = array(
    '#type' => 'textfield',
    '#title' => t('Box columns'),
    '#default_value' => variable_get('nivo_slider_box_columns', '8'),
    '#description' => t('For box animations.'),
  );
  $form['effect']['nivo_slider_box_rows'] = array(
    '#type' => 'textfield',
    '#title' => t('Box rows'),
    '#default_value' => variable_get('nivo_slider_box_rows', '4'),
    '#description' => t('For box animations.'),
  );
  $form['effect']['nivo_slider_animation_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation speed'),
    '#default_value' => variable_get('nivo_slider_animation_speed', '500'),
    '#description' => t('Slide transition speed.'),
  );
  $form['effect']['nivo_slider_pause_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause Time'),
    '#default_value' => variable_get('nivo_slider_pause_time', '3000'),
    '#description' => t('How long each slide will show.'),
  );
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
    '#description' => t('Next & Prev navigation.'),
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
        ':input[name="nivo_slider_directional_navigation"]' => array('value' => '1'),
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
    '#description' => t('1, 2, 3 navigation.'),
  );
  $form['navigation']['nivo_slider_keyboard_navigation'] = array(
    '#type' => 'radios',
    '#title' => t('Keyboard navigation'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_keyboard_navigation', 'true'),
    '#description' => t('Use left & right arrows.'),
  );
  $form['navigation']['nivo_slider_pause_on_hover'] = array(
    '#type' => 'radios',
    '#title' => t('Pause on hover'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_pause_on_hover', 'true'),
    '#description' => t('Stop animation while hovering.'),
  );
  $form['navigation']['nivo_slider_manual_advance'] = array(
    '#type' => 'radios',
    '#title' => t('Manual advance'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => variable_get('nivo_slider_manual_advance', 'false'),
    '#description' => t('Force manual transitions.'),
  );
  $form['navigation']['nivo_slider_previous_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous text'),
    '#default_value' => variable_get('nivo_slider_previous_text', t('Prev')),
    '#description' => t('Previous directional navigation text.'),
  );
  $form['navigation']['nivo_slider_next_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Next text'),
    '#default_value' => variable_get('nivo_slider_next_text', t('Next')),
    '#description' => t('Next directional navigation text.'),
  );

  return system_settings_form($form);
}
