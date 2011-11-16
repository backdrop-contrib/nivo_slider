<?php
/**
 * @file
 * Generate configuration form and save settings.
 */

/**
 * Implements hook_form().
 */
function nivo_slider_slide_configuration_form($form, &$form_state) {
  // Upload
  $form['image_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload a new banner'),
  );

  // Images
  $form['images'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Banner images'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $banners = nivo_slider_get_banners();
  $i = 0;

  foreach ($banners as $image_data) {
    $form['images'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Image !number: !title', array(
        '!number' => $i + 1,
        '!title' => $image_data['image_title'],
        )
      ),
      '#weight' => $i,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      'image' => _nivo_slider_banner_form($image_data), // Image configuration form
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

      if ($image['image_delete']) {
        // Delete banner file
        file_unmanaged_delete($image['image_path']);
        // Delete banner thumbnail file
        file_unmanaged_delete($image['image_thumb']);
      }
      else {
        // Update image
        $settings[] = $image;
      }
    }
  }

  // Check for a new uploaded file, and use that if available.
  if ($file = file_save_upload('image_upload')) {
    $file->status = FILE_STATUS_PERMANENT;
    if ($image = _nivo_slider_save_image($file)) {
      // Put new image into settings
      $settings[] = $image;
    }
  }

  // Save settings
  nivo_slider_set_banners($settings);
}

/**
 * Check if folder is available or create it.
 *
 * @param <string> $dir
 *    Folder to check
 */
function _nivo_slider_check_dir($dir) {
  // Normalize directory name
  $dir = file_stream_wrapper_uri_normalize($dir);

  // Create directory (if not exist)
  file_prepare_directory($dir,  FILE_CREATE_DIRECTORY);
}

/**
 * Save file uploaded by user and generate setting to save.
 *
 * @param <file> $file
 *    File uploaded from user
 *
 * @param <string> $banner_folder
 *    Folder where save image
 *
 * @param <string> $banner_thumb_folder
 *    Folder where save image thumbnail
 *
 * @return <array>
 *    Array with file data.
 *    FALSE on error.
 */
function _nivo_slider_save_image($file, $banner_folder = 'public://banner/', $banner_thumb_folder = 'public://banner/thumb/') {
  // Check directory and create it (if not exist)
  _nivo_slider_check_dir($banner_folder);
  _nivo_slider_check_dir($banner_thumb_folder);

  $parts = pathinfo($file->filename);
  $destination = $banner_folder . $parts['basename'];

  $file->status = FILE_STATUS_PERMANENT;

  // Copy temporary image into banner folder
  if ($img = file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
    // Generate image thumb
    $image = image_load($destination);
    $small_img = image_scale($image, 300, 100);
    $image->source = $banner_thumb_folder . $parts['basename'];
    image_save($image);

    // Create a variable to hold the settings for each image
    $setting = array(
      'image_path' => $destination,
      'image_thumb' => $image->source,
      'image_title' => '',
      'image_description' => '',
      'image_url' => '',
      'image_weight' => 0,
      'image_published' => TRUE,
      'image_visibility' => '*'
    );

    return $setting;
  }
  return FALSE;
}

/**
 * Generate form to mange banner informations
 *
 * @param <array> $image_data
 *   Array with image data
 *
 * @return <array>
 *   Form to manage image informations
 */
function _nivo_slider_banner_form($image_data) {
  $img_form['image_preview'] = array(
    '#markup' => theme('image', array('path' => $image_data['image_thumb'])),
  );
  $img_form['image_path'] = array(
    '#type' => 'hidden',
    '#value' => $image_data['image_path'],
  );
  $img_form['image_thumb'] = array(
    '#type' => 'hidden',
    '#value' => $image_data['image_thumb'],
  );
  $img_form['image_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $image_data['image_title'],
    '#description' => t('Enter a title for the image. The title will be displayed on the banner slide and used as alternative text.'),
  );
  $img_form['image_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $image_data['image_description'],
    '#description' => t('Enter a description of the image. The description will be displayed on the banner slide and used as a rollover title.'),
  );
  $img_form['image_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#default_value' => $image_data['image_url'],
    '#description' => t('Enter a url. If specified, the current image will be linked to the provided url.'),
  );
  $img_form['image_visibility'] = array(
    '#type' => 'textarea',
    '#title' => t('Visibility'),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#default_value' => $image_data['image_visibility'],
  );
  $img_form['image_weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $image_data['image_weight'],
  );
  $img_form['image_published'] = array(
    '#type' => 'checkbox',
    '#title' => t('Published'),
    '#default_value' => $image_data['image_published'],
  );
  $img_form['image_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete image.'),
    '#default_value' => FALSE,
  );
  return $img_form;
}
