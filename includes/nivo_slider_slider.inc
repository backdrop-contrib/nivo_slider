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
function nivo_slider_slider($slider_id) {
  // Get all available slides
  $slider = config_get('nivo_slider.settings', $slider_id);
  $slides = $slider['slides'];
  $options = $slider['options'];
  // Create an array to hold visible slides.
  $visible_slides = array();

  // Determine which slides should be visible.
  foreach ($slides as $slide) {
    // Only display published slides.
    if ($slide['published'] == TRUE) {
      // Convert path to lowercase. This allows comparison of the same path
      // with different case. Ex: /Page, /page, /PAGE.
      $pages = backdrop_strtolower($slide['visibility']);

      // Convert the Backdrop path to lowercase
      $path = backdrop_strtolower(backdrop_get_path_alias($_GET['q']));

      // Compare the lowercase internal and lowercase path alias (if any).
      $page_match = backdrop_match_path($path, $pages);

      if ($path != $_GET['q']) {
        $page_match = $page_match || backdrop_match_path($_GET['q'], $pages);
      }

      // Add the slide if a match was found.
      if ($page_match) {
        $visible_slides[] = $slide;
      }
    }
  }

  // Create a variable to hold the final slider output.
  $output = '';

  // Create a variable to hold any HTML captions.
  $description_html = '';

  foreach ($visible_slides as $slide => $settings) {
    if (isset($settings['description']['value']) && isset($settings['description']['format'])) {
      // Create a variable to hold the slide description
      $description = '';

      // Set the slide description directly when using plain text, otherwise
      // create an HTML caption.
      if ($settings['description']['format'] == 'plain_text') {
        $description = $settings['description']['value'];
      }
      else {
        // WYSIWYG editors are often used with formats other than plain text.
        // These editors often automatically add an empty paragraph (<p></p>),
        // even if no text has been entered, which causes the Nivo Slider
        // jQuery Plugin to display an "empty" caption.
        //
        // To prevent this from happening, strip all HTML tags from the caption,
        // and check to see that some actual text has been entered.
        $description_text_only = strip_tags($settings['description']['value']);

        if (!empty($description_text_only)) {
          // Generate a unique anchor to reference the HTML caption.
          $description = '#htmlcaption-' . $slide;

          // Create the HTML caption markup.
          $html_caption = '';
          $html_caption .= '<div id="htmlcaption-' . $slide . '" class="nivo-html-caption">';
          $html_caption .= check_markup(nivo_slider_translate('slide:' . $slide . ':description', $settings['description']['value']), $settings['description']['format']);
          $html_caption .= '</div>';

          // Add the HTML caption markup.
          $description_html .= $html_caption;
        }
      }

      // Check if the slide has a file ID.
      if (isset($settings['fid'])) {
        // Load the file that corresponds to the file ID.
        $file = file_load($settings['fid']);

        // Create the slide image directly or using image styles.
        if ($options['image_style'] == FALSE) {
          // Create an array of image settings.
          $variables = array(
            'uri' => file_create_url(check_plain($file->uri)),
            'width' => NULL,
            'height' => NULL,
            'title' => check_plain(empty($description_text_only) ? nivo_slider_translate('slide:' . $slide . ':description', $description) : $description),
            'alt' => check_plain(nivo_slider_translate('slide:' . $slide . ':title', $settings['title'])),
            'attributes' => array(
              'class' => array('slide'),
              'id' => 'slide-' . $slide,
              'data-thumb' => file_create_url(check_plain($file->uri)),
              'data-transition' => check_plain($settings['transition']),
            ),
          );

          // Create the slide image.
          $image = theme('image', $variables);
        }
        else {
          // Create an array of image settings.
          $variables = array(
            'style_name' => $options['image_style_slide'],
            'uri' => check_plain($file->uri),
            'title' => check_plain(empty($description_text_only) ? nivo_slider_translate('slide:' . $slide . ':description', $description) : $description),
            'alt' => check_plain(nivo_slider_translate('slide:' . $slide . ':title', $settings['title'])),
            'attributes' => array(
              'class' => array('slide'),
              'id' => 'slide-' . $slide,
              'data-thumb' => image_style_url($options['image_style_thumb'], check_plain($file->uri)),
              'data-transition' => check_plain($settings['transition']),
            ),
          );

          // Create the slide image.
          $image = theme('image_style', $variables);
        }

        // Add a link to the slide image when required.
        $options = array('html' => TRUE);
        if (!empty($settings['newtab'])) {
          $options['attributes'] = array(
            'target' => '_blank'
          );
        }

        $output .= !empty($settings['url']) ? l($image, $settings['url'], $options) : $image;
      }
    }
  }

  // Save all HTML descriptions.
  $slider['options']['banner_html_captions'] = $description_html;
  config_set('nivo_slider.settings', $slider['slider_id'], $slider);
  
  return $output;
}
