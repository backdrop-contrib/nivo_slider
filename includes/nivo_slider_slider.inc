<?php
/**
 * @file
 * Slider creation and generation functions.
 */

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
    if (isset($settings['description']['value']) && isset($settings['description']['format'])) {
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

      // Check if the slide has a file ID
      if (isset($settings['fid'])) {
        // Load the file that corresponds to the file ID
        $file = file_load($settings['fid']);

        // Create an array of image settings
        $variables = array(
          'path' => file_create_url(check_plain($file->uri)),
          'width' => NULL,
          'height' => NULL,
          'alt' => check_plain($settings['title']),
          'title' => check_plain($description),
          'attributes' => array(
            'class' => 'slide',
            'id' => 'slide-' . $slide,
            'data-thumb' => file_create_url(check_plain($file->uri)),
            'data-transition' => check_plain($settings['transition']),
          ),
        );

        // Create the slide image
        $image = theme('image', $variables);

        // Add a link to the slide image when required
        $output .= !empty($settings['url']) ? l($image, $settings['url'], array('html' => TRUE)) : $image;
      }
    }
  }

  // Save all HTML descriptions
  variable_set('nivo_slider_banner_html_captions', $description_html);

  return $output;
}
