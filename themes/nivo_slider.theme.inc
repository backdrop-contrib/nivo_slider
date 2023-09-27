<?php

/**
 * @file
 * Theme and preprocess functions for Nivo Slider.
 */

/**
 * Returns HTML for the slide configuration form into a table.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_nivo_slider_slide_configuration_form($variables) {
  // Get the array of form elements from the available variables
  $form = $variables['form'];

  // Add the tableDrag JavaScript behavior
  backdrop_add_tabledrag('nivo-slider-slides', 'order', 'sibling', 'slide-weight');

  // Create an array to hold the table rows
  $rows = array();
  $slider_id = $form['slider']['#value']['slider_id'];

  // Generate an array of table rows from the available slides
  foreach ($form['images'] as $slide => &$property) {
    if (is_numeric($slide)) {
      // Create an array to hold the table row for the current slide
      $row = array();

      // Add a column containing the slide's title to the current row
      $preview = $property['preview'];
      dpm(backdrop_render($property['fid']));
      $row[] = array(
        'data' => backdrop_render($preview),
      );
      unset($property['preview']);

      // Add a column containing the slide's title to the current row
      $name = $property['name'];
      $row[] = array(
        'data' => backdrop_render($name),
      );
      unset($property['name']);

      // Add a column containing the slide's weight to the current row
      $weight = $property['weight'];
      $row[] = array(
        'data' => backdrop_render($weight),
      );
      unset($property['weight']);

      // Add a column containing the slide's weight to the current row
      $published = $property['published'];
      $row[] = array(
        'data' => backdrop_render($published),
      );
      unset($property['published']);

      // Add a column containing the slide's details to the current row
      $details = $property['details'];
      $row[] = array(
        'data' => backdrop_render($details),
      );
      unset($property['details']);

      // Add a column containing the slide's operations to the current row
      $operations = $property['operations'];
      $row[] = array(
        'data' => backdrop_render($operations),
      );
      unset($property['operations']);
      
      // Store the individual row
      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
      );
    }
  }

  // Build the draggable table if any rows are available
  if (!empty($rows)) {
    // Create an array of table headers
    $headers = array(
      '',
      t('Name'),
      t('Weight'),
      t('Published'),
      t('Information'),
      t('Link'),
    );

    // Over-write the 'order' form element with the draggable table
    $form['order'] = array(
      '#theme' => 'table',
      '#header' => $headers,
      '#rows' => $rows,
      '#attributes' => array(
        'id' => 'nivo-slider-slides',
      ),
    );
  }

  return backdrop_render_children($form);
}

/**
 * Implements template_preprocess_hook().
 */
function template_preprocess_nivo_slider_wrapper(&$variables) {
  $config = config('nivo_slider.settings');
  $slider = $config->get($variables['slider_id']);
  $variables['theme'] = $slider['options']['theme'];
  $variables['banners'] = nivo_slider_slider($variables['slider_id']);
  $variables['html_captions'] = $slider['options']['banner_html_captions'];
}
