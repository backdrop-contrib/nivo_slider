<?php

/**
 * @file
 * Generate configuration form and save settings.
 */

/**
 * Configuration form for slider options.
 */
function nivo_slider_option_configuration_form($form, &$form_state, $slider) {
  backdrop_set_title($slider['slider_id']);
  $options = $slider['options'];
  $form['slider'] = array(
    '#type' => 'value',
    '#value' => $slider,
  );
  
  // Create a list of the currently available themes
  $themes = module_invoke_all('nivo_slider_theme_info');

  // Allow theme information to be altered
  backdrop_alter('nivo_slider_theme_info', $themes);

  $available_themes = array();

  foreach ($themes as $theme => $properties) {
    $available_themes[$theme] = $properties['name'];
  }

  // Theme
  $form['theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#options' => $available_themes,
    '#default_value' => $options['theme'],
    '#description' => t('Select a slider theme. The slider theme determines the overall appearance of the slider.'),
  );

  // Display image style settings if the image module is available
  if (module_exists('image') == TRUE) {
    // Create a list of the currently available image styles
    $image_styles = image_styles();
    $available_themes = array();

    foreach ($image_styles as $style => $properties) {
      $available_image_styles[$style] = $properties['name'];
    }

    // Image Style
    $form['image_style'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use image styles to generate slider images'),
      '#default_value' => $options['image_style'],
      '#description' => t('Easily modify, scale, crop and apply various effects to slider images using <a href="@image-styles">Image styles</a>.', array('@image-styles' => '/admin/config/media/image-styles')),
    );
    $form['image_style_slide'] = array(
      '#type' => 'select',
      '#title' => t('Slide image style'),
      '#options' => $available_image_styles,
      '#default_value' => $options['image_style_slide'],
      '#description' => t('Select an image style to apply to slide images.'),
      '#states' => array(
        'visible' => array(
          ':input[name="image_style"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['image_style_thumb'] = array(
      '#type' => 'select',
      '#title' => t('Tumbnail image style'),
      '#options' => $available_image_styles,
      '#default_value' => $options['image_style_thumb'],
      '#description' => t('Select an image style to apply to slide thumbnail images.'),
      '#states' => array(
        'visible' => array(
          ':input[name="image_style"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  // General
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['general']['random_start'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a random starting slide'),
    '#default_value' => $options['random_start'],
    '#description' => t('Randomly select a slide to begin the slideshow with.'),
  );
  $form['general']['start_slide'] = array(
    '#type' => 'textfield',
    '#title' => t('Starting slide'),
    '#maxlength' => 3,
    '#size' => 3,
    '#default_value' => $options['start_slide'],
    '#description' => t('Specify which slide the slideshow begins with.'),
    '#field_prefix' => t('Slide #'),
    '#states' => array(
      'visible' => array(
        ':input[name="random_start"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Effects
  $form['effects'] = array(
    '#type' => 'fieldset',
    '#title' => t('Effects'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['effects']['effect'] = array(
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
    '#default_value' => $options['effect'],
    '#description' => t('Select an effect. The chosen effect will be used to transition between slides.'),
  );
  $form['effects']['effect_properties'] = array(
    '#type' => 'fieldset',
    '#title' => t('Effect properties'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['effects']['effect_properties']['slices'] = array(
    '#type' => 'textfield',
    '#title' => t('Slices'),
    '#maxlength' => 3,
    '#size' => 3,
    '#default_value' => $options['slices'],
    '#description' => t('Used for slice animations.'),
    '#field_suffix' => t('Slices'),
    '#states' => array(
      'visible' => array(
        array(':input[name="effect"]' => array('value' => 'sliceDown')),
        array(':input[name="effect"]' => array('value' => 'sliceDownLeft')),
        array(':input[name="effect"]' => array('value' => 'sliceUp')),
        array(':input[name="effect"]' => array('value' => 'sliceUpLeft')),
        array(':input[name="effect"]' => array('value' => 'sliceUpDown')),
        array(':input[name="effect"]' => array('value' => 'sliceUpDownLeft')),
        array(':input[name="effect"]' => array('value' => 'fold')),
        array(':input[name="effect"]' => array('value' => 'fade')),
        array(':input[name="effect"]' => array('value' => 'random')),
        array(':input[name="effect"]' => array('value' => 'slideInRight')),
        array(':input[name="effect"]' => array('value' => 'slideInLeft')),
      ),
    ),
  );
  $form['effects']['effect_properties']['box_columns'] = array(
    '#type' => 'textfield',
    '#title' => t('Box columns'),
    '#maxlength' => 3,
    '#size' => 3,
    '#default_value' => $options['box_columns'],
    '#description' => t('Used for box animations.'),
    '#field_suffix' => t('Columns'),
    '#states' => array(
      'visible' => array(
        array(':input[name="effect"]' => array('value' => 'boxRandom')),
        array(':input[name="effect"]' => array('value' => 'boxRain')),
        array(':input[name="effect"]' => array('value' => 'boxRainReverse')),
        array(':input[name="effect"]' => array('value' => 'boxRainGrow')),
        array(':input[name="effect"]' => array('value' => 'boxRainGrowReverse')),
      ),
    ),
  );
  $form['effects']['effect_properties']['box_rows'] = array(
    '#type' => 'textfield',
    '#title' => t('Box rows'),
    '#maxlength' => 3,
    '#size' => 3,
    '#default_value' => $options['box_rows'],
    '#description' => t('Used for box animations.'),
    '#field_suffix' => t('Rows'),
    '#states' => array(
      'visible' => array(
        array(':input[name="effect"]' => array('value' => 'boxRandom')),
        array(':input[name="effect"]' => array('value' => 'boxRain')),
        array(':input[name="effect"]' => array('value' => 'boxRainReverse')),
        array(':input[name="effect"]' => array('value' => 'boxRainGrow')),
        array(':input[name="effect"]' => array('value' => 'boxRainGrowReverse')),
      ),
    ),
  );
  $form['effects']['animation_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation speed'),
    '#maxlength' => 6,
    '#size' => 6,
    '#default_value' => $options['animation_speed'],
    '#description' => t('Enter a time in milliseconds. The animation speed determines how long it takes to transition from one slide to the next.'),
    '#field_suffix' => t('ms'),
  );
  $form['effects']['pause_on_hover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause animation when hovering over the slide'),
    '#default_value' => $options['pause_on_hover'],
    '#description' => t('Disable slide transitions while the user is hovering over the slide.'),
  );

  // Navigation
  $form['navigation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navigation'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['navigation']['directional_navigation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display directional navigation'),
    '#default_value' => $options['directional_navigation'],
    '#description' => t('Display navigation buttons to go back to the previous slide and move forward to the next slide.'),
  );
  $form['navigation']['next_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Next button text'),
    '#default_value' => $options['next_text'],
    '#states' => array(
      'visible' => array(
        ':input[name="directional_navigation"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['navigation']['previous_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous button text'),
    '#default_value' => $options['previous_text'],
    '#states' => array(
      'visible' => array(
        ':input[name="directional_navigation"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['navigation']['control_navigation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display control navigation'),
    '#default_value' => $options['control_navigation'],
    '#description' => t('Display navigation buttons to select an individual slide.'),
  );
  $form['navigation']['control_nav_thumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use thumbnails for control navigation'),
    '#default_value' => $options['control_nav_thumbs'],
    '#description' => t('Use thumbnails of each slide as control navigation buttons. In order for thumbnails to be displayed, a compatible theme is required.'),
    '#states' => array(
      'visible' => array(
        ':input[name="control_navigation"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['navigation']['manual_advance'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable automatic slide transitions'),
    '#default_value' => $options['manual_advance'],
    '#description' => t('For the use to manually navigate between slides.'),
  );
  $form['navigation']['pause_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause time'),
    '#maxlength' => 6,
    '#size' => 6,
    '#default_value' => $options['pause_time'],
    '#description' => t('Enter a time in milliseconds. The pause time determines how long each slide is displayed before transitioning to the next slide.'),
    '#field_suffix' => t('ms'),
    '#states' => array(
      'visible' => array(
        ':input[name="manual_advance"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Configuration form for slider options.
 */
function nivo_slider_option_configuration_form_submit($form, &$form_state) {
  $slider = $form_state['values']['slider'];
  unset($form_state['values']['slider']);
  form_state_values_clean($form_state);
  $slider['options'] = $form_state['values'];
  // Save the slides
  config_set('nivo_slider.settings', $slider['slider_id'], $slider);
}
