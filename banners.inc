<?php
/**
 * @file
 * Generate banner markup.
 */

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
      'path' => $banner['image_path'],
      'title' => t('@image_desc', array('@image_desc' => $banner['image_description'])),
      'alt' => t('@image_title', array('@image_title' => $banner['image_title'])),
      'attributes' => array(
        'class' => 'slide' . ($i != 0 ? ' nivo_slider-hide-no-js' : ''), // hide all the slides except #1
        'id' => 'slide-number-' . $i
      ),
    );
    // Draw image
    $image = theme('image_style', $variables);

    // Remove link if is the same page
    $banner['image_url'] = ($banner['image_url'] == current_path()) ? FALSE  : $banner['image_url'];

    // Add link (if required)
    $output .= $banner['image_url'] ? l($image, $banner['image_url'], array('html' => TRUE)) : $image;
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
    $pages = drupal_strtolower($banner['image_visibility']);

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
