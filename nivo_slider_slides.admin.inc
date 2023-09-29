<?php

/**
 * @file
 * Generate configuration form and save settings.
 */

/**
 * Configuration form for slider slides.
 */
function nivo_slider_slide_image_configuration_form($form, &$form_state, $slider, $slide_number) {
  $slide_setting = $slider['slides'][$slide_number];

  $form_state['slider'] = $slider;
  $form_state['slide_number'] = $slide_number;

  if ($slide_number == 'add') {
    $form['images']['upload'] = array(
      '#type' => 'file',
      '#title' => t('Upload a new slide image'),
    );
  }
  else {
    $form['images']['name'] = array(
      '#markup' => t('Image !number: @title', array(
        '!number' => $slide + 1,
        '@title' => isset($slide_setting['title']) ? $slide_setting['title'] : '',
        )
      ),
    );

    // Load the slide's image file.
    $file = file_load($slide_setting['fid']);

    $variables = array(
      'uri' => $file->uri,
      'style_name' => 'thumbnail',
    );
    $image = theme('image_style', $variables);

    $form['images']['preview'] = array(
      '#markup' =>  $image,
    );
    $form['images']['fid'] = array(
      '#type' => 'hidden',
      '#value' => isset($slide_setting['fid']) ? $slide_setting['fid'] : '',
    );
  }
  $form['images']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($slide_setting['title']) ? $slide_setting['title'] : '',
    '#description' => t('The title is used as alternative text for the slide image.'),
  );
  $form['images']['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Description'),
    '#default_value' => isset($slide_setting['description']['value']) ? $slide_setting['description']['value'] : '',
    '#format' => isset($slide_setting['description']['format']) ? $slide_setting['description']['format'] : NULL,
    '#description' => t('The description will be displayed with the slide image.'),
  );
  $form['images']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Link slide to URL'),
    '#default_value' => isset($slide_setting['url']) ? $slide_setting['url'] : '',
    '#description' => t('Specify a path or an absolute URL. An example path is %blog for the blog page. An example absolute URL is %url for the Backdrop website. %front is the front page.', array('%blog' => 'blog', '%url' => 'http://backdropcms.org', '%front' => '<front>')),
  );
  $form['images']['newtab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open URL in a new tab'),
    '#default_value' => isset($slide_setting['newtab']) ? $slide_setting['newtab'] : '',
  );
  $form['images']['newtab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open URL in a new tab'),
    '#default_value' => isset($slide_setting['newtab']) ? $slide_setting['newtab'] : '',
    '#description' => t('Check this to open URL in a new tab'),
  );
  if (module_exists('i18n')) {
    $languages = language_list('enabled');
    // Take only enabled
    $languages = array_keys($languages[1]);
    $language_array = array();
    foreach ($languages as $key => $language) {
      $language_array[$language] = $language;
    }
    $form['images']['language'] = array(
      '#type' => 'select',
      '#title' => t('Show slide on specific languages'),
      '#options' => $language_array,
      '#default_value' => isset($slide_setting['language']) ? $slide_setting['language'] : '',
      '#empty_option' => '- All -',
      '#description' => t("Specify language on which slide will be shown"),
    );
  }
  $form['images']['visibility'] = array(
    '#type' => 'textarea',
    '#title' => t('Show slide on specific pages'),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#default_value' => isset($slide_setting['visibility']) ? $slide_setting['visibility'] : '*',
  );
  $form['images']['transition'] = array(
    '#type' => 'select',
    '#title' => t('Transition'),
    '#options' => array(
      '' => t('- Default -'),
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
    '#description' => t('Select a transition. Selecting an option other than %default will force this slide to use the selected transition every time it appears. It overrides all other effect slide_setting.', array('%default' => '- Default -')),
    '#default_value' => isset($slide_setting['transition']) ? $slide_setting['transition'] : '',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#validate' => array(),
    '#submit' => array('nivo_slider_slide_image_configuration_form_cancel'),
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save slide'), //empty($layout->is_new) ? t('Save layout') : t('Create layout'),
  );

  return $form;
}

/**
 * Save settings data.
 */
function nivo_slider_slide_image_configuration_form_cancel($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/nivo-slider/' . $form_state['slider']['slider_id'];
}

/**
 * Save settings data.
 */
function nivo_slider_slide_image_configuration_form_validate($form, &$form_state) {

  // Create a new slide if an image was uploaded.
  if ($file = file_save_upload('upload')) {
    // Store slide images in a folder named 'banner'
    $banner_folder = 'public://' . config_get('nivo_slider.settings', 'dir_name');

    // Create the banner directory if it does not currently exist.
    file_prepare_directory($banner_folder, FILE_CREATE_DIRECTORY);

    // Find information about the file.
    $parts = pathinfo($file->filename);

    // Create a destination for the slide image.
    $destination = $banner_folder . '/' . $parts['basename'];

    // Indicate that the slide image is permanent.
    $file->status = FILE_STATUS_PERMANENT;

    // Copy the slide image into the banner folder.
    $file = file_copy($file, $destination, FILE_EXISTS_RENAME);
    $form_state['new_fid'] = $file->fid;
  }
}

/**
 * Save settings data.
 */
function nivo_slider_slide_image_configuration_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $slide_number = $form_state['slide_number'];
  if ($slide_number != 'add') {
    $form_state['slider']['slides'][$slide_number] = array_merge($form_state['slider']['slides'][$slide_number], $form_state['values']);
  } else {
    $form_state['values']['fid'] = $form_state['new_fid'];
    $form_state['slider']['slides'][] = $form_state['values'];
  }

  config_set('nivo_slider.settings', $form_state['slider']['slider_id'], $form_state['slider']);
}

/**
 * Configuration form for slider slides.
 */
function nivo_slider_slide_configuration_form($form, &$form_state, $slider) {
  $form['#attached'] = array(
    'library' => array(
      array('system', 'backdrop.ajax'),
      array('system', 'backdrop.announce'),
    ),
  );

  backdrop_set_title($slider['slider_name']);

  // Draggable table.
  $form['order'] = array();

  // Vertical tab.
  $form['images'] = array(
    '#type' => 'item',
    '#title' => t('Slider images'),
    '#tree' => TRUE,
  );

  $form['slider'] = array(
    '#type' => 'value',
    '#value' => $slider,
  );

  // Get all available slides.
  $slides = isset($slider['slides']) ? $slider['slides'] : array();
  $max_weigh_delta = count($slides) + 10;

  // Sort the slides by weight
  backdrop_sort($slides, array('weight'));

  // Create a vertical tab for each slide.
  foreach ($slides as $slide => $settings) {
    $form['images'][$slide]['weight'] = array(
      '#type' => 'weight',
      '#delta' => $max_weigh_delta,
      '#default_value' => isset($settings['weight']) ? $settings['weight'] : 1,
      '#attributes' => array(
        'class' => array('slide-weight'),
      ),
    );
    $form['images'][$slide]['published'] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($settings['published']) ? $settings['published'] : '',
    );

    $links = array();
    $slider_id = $slider['slider_id'];
    $links['configure'] = array(
      'title' => t('Configure'),
      'href' => "admin/structure/nivo-slider/$slider_id/$slide",
      'attributes' => array(
        'class' => array('use-ajax'),
        'data-dialog' => TRUE,
        'data-dialog-options' => json_encode(array('dialogClass' => 'nivo-dialog')),
      ),
    );
    $links['delete'] = array(
      'title' => t('Delete'),
      'href' => "admin/structure/nivo-slider/$slider_id/$slide/delete",
    );

    $form['images'][$slide]['operations'] = array(
      '#type' => 'dropbutton',
      '#links' => $links,
    );

    // Load the slide's image file.
    $file = file_load($settings['fid']);

    $variables = array(
      'uri' => $file->uri,
      'style_name' => 'thumbnail',
    );
    $image = theme('image_style', $variables);

    $form['images'][$slide]['preview'] = array(
      '#markup' =>  l($image, 'file/' . $settings['fid'], array('html' => TRUE)),
    );

    $form['images'][$slide]['fid'] = array(
      '#type' => 'hidden',
      '#value' => isset($settings['fid']) ? $settings['fid'] : '',
    );

    $details = nivo_slider_slide_get_details($settings);
    $form['images'][$slide]['details'] = array(
      '#type' => 'details',
      '#summary' => t('Image information'),
      '#details' => $details,
    );

    if (module_exists('i18n')) {
      $languages = language_list('enabled');
      // Take only enabled
      $languages = array_keys($languages[1]);
      $language_array = array();
      foreach ($languages as $key => $language) {
        $language_array[$language] = $language;
      }
      $form['images'][$slide]['language'] = array(
        '#type' => 'select',
        '#title' => t('Show slide on specific languages'),
        '#options' => $language_array,
        '#default_value' => isset($settings['language']) ? $settings['language'] : '',
        '#empty_option' => '- All -',
        '#description' => t("Specify language on which slide will be shown"),
      );
    }
  }

  // Add a theme function to theme the form.
  $form['#theme'][] = 'nivo_slider_slide_configuration_form';

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


/**
 * Save settings data.
 */
function nivo_slider_slide_configuration_form_submit($form, &$form_state) {
  // Create an array to hold slides.
  $slides = array();

  // Process the available slides.
  foreach ($form_state['values']['images'] as $slide => $settings) {
    // Ensure that only slide settings are processed
    if (is_numeric($slide)) {
      if (is_array($settings)) {
        $slides[] = $settings;
      }
    }
  }

  // Update/create translation source for user defined slide strings.
  if (function_exists('i18n_string_update')) {
    nivo_slider_locale_refresh($slides);
  }

  // Save the slides.
  $save_slides = $form_state['values']['slider'];
  $save_slides['slides'] = $slides;
  config_set('nivo_slider.settings', $form_state['values']['slider']['slider_id'], $save_slides);
}

/**
 * Get slide details.
 */
function nivo_slider_slide_get_details($slide_info) {
  $output = '';
  $output .= '<div>' . t('Title: ');
  $output .= !empty($slide_info['title']) ? check_plain($slide_info['title']) : t('Default');
  $output .= '</div>';
  $output .= '<div>' . t('Description: ');
  $output .= !empty($slide_info['description']['value']) ? check_plain($slide_info['description']['value']) : t('No description');
  $output .= '</div>';

  if (!empty($slide_info['url'])) {
    $output .= '<div>' . t('URL: ');
    $output .= $slide_info['url'];
    $output .= '</div>';
    $output .= '<div>' . t('New tab: ');
    $output .= empty($slide_info['new_tab']) ? t('Opens in current tab') : t('Opens in new tab');
    $output .= '</div>';
  }
  else {
    $output .= '<div>' . t('URL: ');
    $output .= t('Not linked to URL');
    $output .= '</div>';
  }

  $output .= '<div>' . t('Visibility: ');
  $output .= !empty($slide_info['visibility'] && $slide_info['visibility'] != '*') ? $slide_info['visibility'] : t('Shown on all pages');
  $output .= '</div>';
  $output .= '<div>' . t('Transition: ');
  $output .= !empty($slide_info['transition']) ? $slide_info['transition'] : t('No transition');
  $output .= '</div>';

  return $output;
}

/**
 * Delete slide confirm form.
 */
function nivo_slider_slide_image_delete_confirm_form($form, &$form_state, $slider, $slide) {
  $slide_info = $slider['slides'][$slide];

  // Load the slide's image file.
  $file = file_load($slide_info['fid']);

  $variables = array(
    'uri' => $file->uri,
    'style_name' => 'medium',
  );
  $image = theme('image_style', $variables);

  $form['preview'] = array(
    '#markup' =>  '<div>' . $image . '</div>',
  );

  $form_state['slider'] = $slider;
  $form_state['slide'] = $slide;

  $slide_name = !empty($slide_info['title']) ? $slide_info['title'] : 'number ' . ($slide + 1);
  $question = t('Delete slide @slide_name?', array('@slide_name' => $slide_name));

  return confirm_form($form, $question, 'admin/structure/nivo-slider/' . $slider['slider_id']);
}


/**
 * Delete slide confirm form submit.
 */
function nivo_slider_slide_image_delete_confirm_form_submit($form, &$form_state) {
  $slider = $form_state['slider'];
  $slide = $form_state['slide'];

  file_delete($slider['slides'][$slide]['fid']);
  unset($slider['slides'][$slide]);
  config_set('nivo_slider.settings', $slider['slider_id'], $slider);

  $form_state['redirect'] = 'admin/structure/nivo-slider/' . $slider['slider_id'];
}

