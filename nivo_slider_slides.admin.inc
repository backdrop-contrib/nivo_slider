<?php
/**
 * @file
 * Generate configuration form and save settings.
 */

// Include the banner generation functions
include_once('banners.inc');

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

  $banners = nivo_slider_get_slider_settings();
  $i = 0;

  foreach ($banners as $data) {
    $form['images'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Image !number: !title', array(
        '!number' => $i + 1,
        '!title' => $data['title'],
        )
      ),
      '#weight' => $i,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      'image' => array(),
    );
    $form['images'][$i]['image']['preview'] = array(
      '#markup' => theme('image', array('path' => image_style_url('nivo_slider_slide', ($data['path'])))),
    );
    $form['images'][$i]['image']['path'] = array(
      '#type' => 'hidden',
      '#value' => $data['path'],
    );
    $form['images'][$i]['image']['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $data['title'],
      '#description' => t('The title is used as alternative text for the slide image.'),
    );
    $form['images'][$i]['image']['description'] = array(
      '#type' => 'text_format',
      '#title' => t('Description'),
      '#default_value' => $data['description']['value'],
      '#format' => isset($data['description']['format']) ? $data['description']['format'] : NULL,
      '#description' => t('The description will be displayed with the slide image.'),
    );
    $form['images'][$i]['image']['url'] = array(
      '#type' => 'textfield',
      '#title' => t('Link slide to URL'),
      '#default_value' => $data['url'],
    );
    $form['images'][$i]['image']['visibility'] = array(
      '#type' => 'textarea',
      '#title' => t('Show slide on specific pages'),
      '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
      '#default_value' => $data['visibility'],
    );
    $form['images'][$i]['image']['transition'] = array(
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
    	'#default_value' => $data['transition'],
    );
    $form['images'][$i]['image']['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $data['weight'],
    );
    $form['images'][$i]['image']['published'] = array(
      '#type' => 'checkbox',
      '#title' => t('Published'),
      '#default_value' => $data['published'],
    );
    $form['images'][$i]['image']['delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete slide'),
      '#default_value' => FALSE,
    );
    $i++;
  }

  $form['#submit'][] = 'nivo_slider_settings_submit';

  return system_settings_form($form);
}

/**
 * Save settings data.
 */
function nivo_slider_settings_submit($form, &$form_state) {
  $settings = array();

  // Update image field
  foreach ($form_state['input']['images'] as $image) {
    if (is_array($image)) {
      $image = $image['image'];

      if ($image['delete']) {
        // Delete banner file
        file_unmanaged_delete($image['path']);
      }
      else {
        // Update image
        $settings[] = $image;
      }
    }
  }

  // Check for a new uploaded file, and use that if available.
  if ($file = file_save_upload('upload')) {
    $file->status = FILE_STATUS_PERMANENT;
    if ($image = _nivo_slider_save_image($file)) {
      // Put new image into settings
      $settings[] = $image;
    }
  }

  // Save settings
  nivo_slider_set_slider_settings($settings);
}
