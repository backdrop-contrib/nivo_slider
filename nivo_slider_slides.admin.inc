<?php
/**
 * @file
 * Generate configuration form and save settings.
 */

// Include the slider functions
include_once('slider.inc');

/**
 * Implements hook_form().
 */
function nivo_slider_slide_configuration_form($form, &$form_state) {
  // Upload
  $form['upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload a new slide image'),
  );

  // Images
  $form['images'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Slider images'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  // Get all available slides
  $slides = variable_get('nivo_slider_banner_settings', array());

  // Create a vertical tab filled with slide settings for each slide
  foreach ($slides as $slide => $settings) {
    $form['images'][$slide] = array(
      '#type' => 'fieldset',
      '#title' => t('Image !number: !title', array(
        '!number' => $slide + 1,
        '!title' => $settings['title'],
        )
      ),
      '#weight' => $slide,
    );
    $form['images'][$slide]['preview'] = array(
      '#markup' => theme('image', array('path' => image_style_url('nivo_slider_slide', ($settings['path'])))),
    );
    $form['images'][$slide]['path'] = array(
      '#type' => 'hidden',
      '#value' => $settings['path'],
    );
    $form['images'][$slide]['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $settings['title'],
      '#description' => t('The title is used as alternative text for the slide image.'),
    );
    $form['images'][$slide]['description'] = array(
      '#type' => 'text_format',
      '#title' => t('Description'),
      '#default_value' => $settings['description']['value'],
      '#format' => isset($settings['description']['format']) ? $settings['description']['format'] : NULL,
      '#description' => t('The description will be displayed with the slide image.'),
    );
    $form['images'][$slide]['url'] = array(
      '#type' => 'textfield',
      '#title' => t('Link slide to URL'),
      '#default_value' => $settings['url'],
    );
    $form['images'][$slide]['visibility'] = array(
      '#type' => 'textarea',
      '#title' => t('Show slide on specific pages'),
      '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
      '#default_value' => $settings['visibility'],
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
    	'#default_value' => $settings['transition'],
    );
    $form['images'][$slide]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $settings['weight'],
    );
    $form['images'][$slide]['published'] = array(
      '#type' => 'checkbox',
      '#title' => t('Published'),
      '#default_value' => $settings['published'],
    );
    $form['images'][$slide]['delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete slide'),
      '#default_value' => FALSE,
    );
  }

  // Add a submit handler to save the slide settings
  $form['#submit'][] = 'nivo_slider_settings_submit';

  return system_settings_form($form);
}

/**
 * Save settings data.
 */
function nivo_slider_settings_submit($form, &$form_state) {
  // Create an array to hold slides
  $slides = array();

  // Create a file object to hold the slide image file
  $file = new stdClass;

  // Process the available slides
  foreach ($form_state['input']['images'] as $slide => $settings) {
    // Delete the slide if required otherwise add it to the array of slides
    if ($settings['delete']) {
      // Find the URI of the slide to be deleted
      $uri = $settings['path'];

      // Find the file ID of the file with the URI
      $fid = db_query('SELECT fid FROM {file_managed} WHERE uri = :uri', array(':uri' => $uri))->fetchField();

      // Load the file with the file ID
      if (!empty($fid)) {
        $file_object = file_load($fid);
      }
      else {
        $file_object = FALSE;
      }

      // Delete the file
      if (!empty($file_object)) {
        $result = file_delete($file_object);
      }
    }
    else {
      $slides[] = $settings;
    }
  }

  // Create a new slide if an image was uploaded
  if ($file = file_save_upload('upload')) {
    // Save the file and get the new path to the slide image
    $path_to_slide_image = _nivo_slider_save_image($file);

    // Create a new slide
    $slides[] = array(
      'path' => $path_to_slide_image,
      'title' => '',
      'description' => array(
        'value' => '',
        'format' => 'plain_text',
      ),
      'url' => '',
      'visibility' => '*',
      'transition' => '',
      'weight' => 1,
      'published' => TRUE,
      'delete' => '',
    );
  }

  // Sort the slides by weight
  usort($slides, 'drupal_sort_weight');

  // Save the slides
  variable_set('nivo_slider_banner_settings', $slides);
}
