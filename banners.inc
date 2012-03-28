<?php
/**
 * @file
 * Banner creation and generation functions.
 */

/**
 * Save file uploaded by user and generate setting to save.
 *
 * @param <file> $file
 *    File uploaded from user
 *
 * @param <string> $banner_folder
 *    Folder where save image
 *
 * @return <array>
 *    Array with file data.
 *    FALSE on error.
 */
function _nivo_slider_save_image($file, $banner_folder = 'public://banner/') {
  // Normalize directory name
  $banner_folder = file_stream_wrapper_uri_normalize($banner_folder);

  // Create directory (if not exist)
  file_prepare_directory($banner_folder,  FILE_CREATE_DIRECTORY);

  $parts = pathinfo($file->filename);
  $destination = $banner_folder . $parts['basename'];

  $file->status = FILE_STATUS_PERMANENT;

  // Copy temporary image into banner folder
  if ($img = file_copy($file, $destination, FILE_EXISTS_REPLACE)) {

    // Create a variable to hold the settings for each image
    $setting = array(
      'path' => $destination,
      'title' => '',
      'description' => '',
      'url' => '',
      'transition' => '',
      'weight' => 0,
      'published' => TRUE,
      'visibility' => '*'
    );

    return $setting;
  }
  return FALSE;
}

/**
 * Gets slider settings.
 *
 * @return <array>
 *    Settings to get
 */
function nivo_slider_get_slider_settings() {
  return variable_get('nivo_slider_banner_settings', array());
}

/**
 * Sets slider settings.
 *
 * @param <array> $value
 *    Settings to save
 */
function nivo_slider_set_slider_settings($value) {
  // Create list of banner to return
  $banners_value = array();
  foreach ($value as $banner) {
    $banners_value[] = $banner;
  }

  // Sort image by weight
  usort($banners_value, 'drupal_sort_weight');

  variable_set('nivo_slider_banner_settings', $banners_value);
}

/**
 * Generates slider markup.
 *
 * @return <string>
 *    Slider HTML markup.
 */
function nivo_slider_slider() {
  $banners = nivo_slider_get_slider_settings();
  $display_banners = array();

  // Current path alias
  $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));

  // Check visibility for each banner
  foreach ($banners as $banner) {
    // Convert path to lowercase. This allows comparison of the same path
    // with different case. Ex: /Page, /page, /PAGE.
    $pages = drupal_strtolower($banner['visibility']);

    // Convert the Drupal path to lowercase
    $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));

    // Compare the lowercase internal and lowercase path alias (if any).
    $page_match = drupal_match_path($path, $pages);

    if ($path != $_GET['q']) {
      $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
    }

    // Add banner to visible banner
    if ($page_match) {
      $display_banners[] = $banner;
    }
  }

  // Create a variable to hold the final banner output
  $output = '';

  // Create a variable to hold any HTML captions
  $description_html = '';

  foreach ($display_banners as $i => $banner) {
    // Create a variable to hold the banner description
    $description = '';

    // Check the text format of the banner description
    $text_format = $banner['description']['format'];

    // Set the slide description directly when using plain text otherwise create an
    // HTML caption
    if ($text_format == 'plain_text') {
      $description = $banner['description']['value'];
    }
    else {
      $description = '#htmlcaption-' . $i;

      $html_caption = '';
      $html_caption .= '<div id="htmlcaption-' . $i . '" class="nivo-html-caption">';
      $html_caption .= check_markup($banner['description']['value'], $banner['description']['format']);
      $html_caption .= '</div>';

      $description_html .= $html_caption;
    }

    // Create an array of settings to use while theming
    $variables = array(
      'style_name' => 'nivo_slider_slide',
      'path' => check_plain($banner['path']),
      'title' => check_plain($description),
      'alt' => check_plain($banner['title']),
      'attributes' => array(
        'class' => 'slide',
        'id' => 'slide-' . $i,
        'rel' => image_style_url('nivo_slider_thumbnail', check_plain($banner['path'])),
        'data-transition' => check_plain($banner['transition']),
      ),
    );

    // Draw image
    $image = theme('image_style', $variables);

    // Remove link if is the same page
    $banner['url'] = ($banner['url'] == current_path()) ? FALSE  : $banner['url'];

    // Add link (if required)
    $output .= $banner['url'] ? l($image, $banner['url'], array('html' => TRUE)) : $image;
  }

  // Save all HTML descriptions
  variable_set('nivo_slider_banner_html_captions', $description_html);

  // Set the slider's dimensions to those of the first banner image
  if (isset($display_banners[0])) {
    $path = image_style_path('nivo_slider_slide', $display_banners[0]['path']); // Get the path to the banner's first image
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
