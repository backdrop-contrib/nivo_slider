<?php
/**
 * @file
 * Banner creation and generation functions.
 */

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
function _nivo_slider_save_image($file, $banner_folder = 'public://banner/') {
  // Create a directory to store the banner images if one doesn't already exist
  _nivo_slider_check_dir($banner_folder);

  $parts = pathinfo($file->filename);
  $destination = $banner_folder . $parts['basename'];

  $file->status = FILE_STATUS_PERMANENT;

  // Copy temporary image into banner folder
  if ($img = file_copy($file, $destination, FILE_EXISTS_REPLACE)) {

    // Create a variable to hold the settings for each image
    $setting = array(
      'image_path' => $destination,
      'image_title' => '',
      'image_description' => '',
      'image_url' => '',
      'image_transition' => '',
      'image_weight' => 0,
      'image_published' => TRUE,
      'image_visibility' => '*'
    );

    return $setting;
  }
  return FALSE;
}

/**
 * Get banner settings.
 *
 * @param <bool> $all
 *    Return all banners or only active.
 *
 * @return <array>
 *    Settings information
 */
function nivo_slider_get_banners($all = TRUE) {
  // Get all banners
  $banners = variable_get('nivo_slider_banner_settings', array());

  // Create list of banner to return
  $banners_value = array();
  foreach ($banners as $banner) {
    if ($all || $banner['image_published']) {
      // Add weight param to use `drupal_sort_weight`
      $banner['weight'] = $banner['image_weight'];
      $banners_value[] = $banner;
    }
  }

  // Sort image by weight
  usort($banners_value, 'drupal_sort_weight');

  return $banners_value;
}

/**
 * Set banner settings.
 *
 * @param <array> $value
 *    Settings to save
 */
function nivo_slider_set_banners($value) {
  variable_set('nivo_slider_banner_settings', $value);
}

/**
 * Generate markup for nivo_slider's banners.
 *
 * @return <string>
 *    HTML markup to printo into page
 */
function nivo_slider_banners_markup($banners) {
  if ($banners) {
    // Generate HTML markup for banners
    return nivo_slider_banner_markup($banners);
  }
  else {
    return '';
  }
}

/**
 * Generate banners markup.
 *
 * @return <string>
 *    HTML code to display banner markup.
 */
function nivo_slider_banner_markup($banners) {
  $output = '';

  foreach ($banners as $i => $banner) {
    $variables = array(
      'style_name' => 'nivo_slider_slide',
      'path' => check_plain($banner['image_path']),
      'title' => t('@image_desc', array('@image_desc' => $banner['image_description'])),
      'alt' => t('@image_title', array('@image_title' => $banner['image_title'])),
      'attributes' => array(
        'class' => 'slide' . $i,
        'id' => 'slide-number-' . $i,
        'rel' => image_style_url('nivo_slider_thumbnail', check_plain($banner['image_path'])),
        'data-transition' => check_plain($banner['image_transition']),
      ),
    );
    // Draw image
    $image = theme('image_style', $variables);

    // Remove link if is the same page
    $banner['image_url'] = ($banner['image_url'] == current_path()) ? FALSE  : $banner['image_url'];

    // Add link (if required)
    $output .= $banner['image_url'] ? l($image, $banner['image_url'], array('html' => TRUE)) : $image;
  }

  // Set the slider's dimensions to those of the first banner image
  if (isset($banners[0])) {
    $path = image_style_path('nivo_slider_slide', $banners[0]['image_path']); // Get the path to the banner's first image
    $dimensions = image_get_info($path); // Find the image's dimensions

    $css = '';
    $css .= '.nivoSlider {';
    $css .= 'width: ' . $dimensions['width'] . 'px;'; // Determine image width
    $css .= 'height: ' . $dimensions['height'] . 'px;'; // Determine image height
    $css .= '}';

    drupal_add_css($css, array(
      'type' => 'inline',
      )
    );
  }
  return $output;
}

/**
 * Get banner to show into current page in accord with settings
 *
 * @return <array>
 *    Banners to show
 */
function nivo_slider_show_banners() {
  $banners = nivo_slider_get_banners(FALSE);
  $display_banners = array();

  // Current path alias
  $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));

  // Check visibility for each banner
  foreach ($banners as $banner) {
    // Pages
    $pages = check_plain(drupal_strtolower($banner['image_visibility']));

    // Check path for alias, and (if required) for path
    $page_match = drupal_match_path($path, $pages);
    if ($path != $_GET['q']) {
      $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
    }

    // Add banner to visible banner
    if ($page_match) {
      $display_banners[] = $banner;
    }
  }
  return $display_banners;
}
