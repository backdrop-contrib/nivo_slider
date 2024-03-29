<?php

/**
 * @file
 * Generate configuration form and save settings.
 */

/**
 * Configuration form for slider slides.
 */
function nivo_slider_slide_configuration_form($form, &$form_state, $slider) {
  backdrop_set_title($slider['slider_name']);
  // Upload.
  $form['upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload a new slide image'),
  );
  $form['submit2'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  // Draggable table.
  $form['order'] = array();

  // Vertical tab.
  $form['images'] = array(
    '#type' => 'vertical_tabs',
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
    $form['images'][$slide] = array(
      '#type' => 'fieldset',
      '#title' => t('Image !number: @title', array(
        '!number' => $slide + 1,
        '@title' => isset($settings['title']) ? $settings['title'] : '',
        )
      ),
      '#weight' => $slide,
    );
    $form['images'][$slide]['name'] = array(
      '#markup' => t('Image !number: @title', array(
        '!number' => $slide + 1,
        '@title' => isset($settings['title']) ? $settings['title'] : '',
        )
      ),
    );
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
    $form['images'][$slide]['delete'] = array(
      '#type' => 'checkbox',
      '#default_value' => FALSE,
    );

    // Load the slide's image file.
    $file = file_load($settings['fid']);

    // Create a preview image of the slide using an image style if appropriate.
    if ($slider['options']['image_style'] == FALSE) {
      $variables = array(
        'uri' => file_create_url($file->uri),
      );
      $image = theme('image', $variables);
    }
    else {
      $variables = array(
        'uri' => $file->uri,
        'style_name' => $slider['options']['image_style_slide']
      );
      $image = theme('image_style', $variables);
    }

    $form['images'][$slide]['preview'] = array(
      '#markup' =>  $image,
    );
    $form['images'][$slide]['fid'] = array(
      '#type' => 'hidden',
      '#value' => isset($settings['fid']) ? $settings['fid'] : '',
    );
    $form['images'][$slide]['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => isset($settings['title']) ? $settings['title'] : '',
      '#description' => t('The title is used as alternative text for the slide image.'),
    );
    $form['images'][$slide]['description'] = array(
      '#type' => 'text_format',
      '#title' => t('Description'),
      '#default_value' => isset($settings['description']['value']) ? $settings['description']['value'] : '',
      '#format' => isset($settings['description']['format']) ? $settings['description']['format'] : NULL,
      '#description' => t('The description will be displayed with the slide image.'),
    );
    $form['images'][$slide]['url'] = array(
      '#type' => 'textfield',
      '#title' => t('Link slide to URL'),
      '#default_value' => isset($settings['url']) ? $settings['url'] : '',
      '#description' => t('Specify a path or an absolute URL. An example path is %blog for the blog page. An example absolute URL is %url for the Backdrop website. %front is the front page.', array('%blog' => 'blog', '%url' => 'http://backdropcms.org', '%front' => '<front>')),
    );
    $form['images'][$slide]['newtab'] = array(
      '#type' => 'checkbox',
      '#title' => t('Open URL in a new tab'),
      '#default_value' => isset($settings['newtab']) ? $settings['newtab'] : '',
    );
    $form['images'][$slide]['visibility'] = array(
      '#type' => 'textarea',
      '#title' => t('Show slide on specific pages'),
      '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
      '#default_value' => isset($settings['visibility']) ? $settings['visibility'] : '*',
    );
    $form['images'][$slide]['transition'] = array(
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
      '#description' => t('Select a transition. Selecting an option other than %default will force this slide to use the selected transition every time it appears. It overrides all other effect settings.', array('%default' => '- Default -')),
      '#default_value' => isset($settings['transition']) ? $settings['transition'] : '',
    );
  }

  // Add a theme function to theme the form.
  $form['#theme'][] = 'nivo_slider_slide_configuration_form';

  $form['submit'] = array(
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

  // Create a file object to hold the slide image file.
  $file = entity_create('file', array());

  // Process the available slides.
  foreach ($form_state['values']['images'] as $slide => $settings) {
    // Ensure that only slide settings are processed
    if (is_numeric($slide)) {
      if (is_array($settings)) {
        // Delete the slide if required otherwise add it to the array of slides.
        if ($settings['delete']) {
          // Load the file with the file ID.
          if (isset($settings['fid'])) {
            file_delete($settings['fid']);
          }
        }
        else {
          $slides[] = $settings;
        }
      }
    }
  }

  // Update/create translation source for user defined slide strings.
  if (function_exists('i18n_string_update')) {
    nivo_slider_locale_refresh($slides);
  }

  // Create a new slide if an image was uploaded.
  if ($file = file_save_upload('upload')) {
    // Store slide images in a folder named 'banner'.
    $banner_folder = 'public://banner';

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

    // Create a new slide.
    $slides[] = array(
      'fid' => $file->fid,
      'title' => '',
      'description' => array(
        'value' => '',
        'format' => filter_fallback_format(),
      ),
      'url' => '',
      'visibility' => '*',
      'transition' => '',
      'weight' => 1,
      'published' => 1,
      'delete' => 0,
    );

    // Don't redirect if coming from contextual links.
    unset($_GET['destination']);
  }
  // Save the slides.
  $save_slides = $form_state['values']['slider'];
  $save_slides['slides'] = $slides;
  config_set('nivo_slider.settings', $form_state['values']['slider']['slider_id'], $save_slides);
}
