<?php
/**
 * @file
 * Slider creation and generation functions.
 */

/**
 * Save uploaded slide images.
 *
 * @param <file> $file
 *    Uploaded slide image file
 *
 * @return <string>
 *    Path to saved image
 *    FALSE on error
 */
function _nivo_slider_save_image($file) {
  // Store slide images in a folder named 'banner'
  $banner_folder = 'public://banner';

  // Create the banner directory if it does not currently exist
  file_prepare_directory($banner_folder, FILE_CREATE_DIRECTORY);

  // Find information about the file
  $parts = pathinfo($file->filename);

  // Create a destination for the slide image
  $destination = $banner_folder . '/' . $parts['basename'];

  // Indicate that the slide image is permanent
  $file->status = FILE_STATUS_PERMANENT;

  // Copy the slide image into the banner folder
  if (file_copy($file, $destination, FILE_EXISTS_REPLACE != FALSE)) {
    return $destination;
  }

  return FALSE;
}

/**
 * Generates slider markup.
 *
 * @return <string>
 *    Slider HTML markup
 */
function nivo_slider_slider() {
  // Get all available slides
  $slides = variable_get('nivo_slider_banner_settings', array());

  // Create an array to hold visible slides
  $visible_slides = array();

  // Find the current path alias
  $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));

  // Determine which slides should be visible
  foreach ($slides as $slide) {
    // Only display published slides
    if ($slide['published'] == TRUE) {
      // Convert path to lowercase. This allows comparison of the same path
      // with different case. Ex: /Page, /page, /PAGE.
      $pages = drupal_strtolower($slide['visibility']);

      // Convert the Drupal path to lowercase
      $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));

      // Compare the lowercase internal and lowercase path alias (if any).
      $page_match = drupal_match_path($path, $pages);

      if ($path != $_GET['q']) {
        $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
      }

      // Add the slide if a match was found
      if ($page_match) {
        $visible_slides[] = $slide;
      }
    }
  }

  // Create a variable to hold the final slider output
  $output = '';

  // Create a variable to hold any HTML captions
  $description_html = '';

  foreach ($visible_slides as $slide => $settings) {
    // Create a variable to hold the slide description
    $description = '';

    // Set the slide description directly when using plain text otherwise create an
    // HTML caption
    if ($settings['description']['format'] == 'plain_text') {
      $description = $settings['description']['value'];
    }
    else {
      // Generate a unique anchor to reference the HTML caption
      $description = '#htmlcaption-' . $slide;

      // Create the HTML caption markup
      $html_caption = '';
      $html_caption .= '<div id="htmlcaption-' . $slide . '" class="nivo-html-caption">';
      $html_caption .= check_markup($settings['description']['value'], $settings['description']['format']);
      $html_caption .= '</div>';

      // Add the HTML caption markup
      $description_html .= $html_caption;
    }

    // Create an array of image settings
    $variables = array(
      'style_name' => 'nivo_slider_slide',
      'path' => check_plain($settings['path']),
      'title' => check_plain($description),
      'alt' => check_plain($settings['title']),
      'attributes' => array(
        'class' => 'slide',
        'id' => 'slide-' . $slide,
        'rel' => image_style_url('nivo_slider_thumbnail', check_plain($settings['path'])),
        'data-transition' => check_plain($settings['transition']),
      ),
    );

    // Create the slide image
    $image = theme('image_style', $variables);

    // Add a link to the slide image when required
    $output .= !empty($settings['url']) ? l($image, $settings['url'], array('html' => TRUE)) : $image;
  }

  // Save all HTML descriptions
  variable_set('nivo_slider_banner_html_captions', $description_html);

  // Set the slider's dimensions to those of the first slider image
  if (isset($visible_slides[0])) {
    // Get the path to the first slider image
    $path = image_style_path('nivo_slider_slide', $visible_slides[0]['path']);

    // Find the image's dimensions
    $dimensions = image_get_info($path);

    // Create a CSS selector to assign the height and width to the slider
    $css = '.nivoSlider {width: ' . $dimensions['width'] . 'px; height: ' . $dimensions['height'] . 'px;}';

    // Add the CSS
    drupal_add_css($css, array('type' => 'inline'));
  }

  return $output;
}
