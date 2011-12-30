<?php

/**
 * @file
 * Default theme implementation for displaying a banner.
 *
 * A banner wraps slides in HTML, which provides an anchor for the Nivo Slider
 * to target and create a slideshow with the appropriate settings and theming.
 *
 * Available variables:
 * - $theme: String containing the name of the currently active theme.
 * - $banners: String of HTML representing a banner.
 *
 * @see template_preprocess()
 * @see template_process()
 */
?>
<?php if ($banners): ?>
  <div class="slider-wrapper theme-<?php print $theme; ?>">
    <div class="ribbon"></div>
    <div id="slider" class="nivoSlider">
      <?php print $banners; ?>
    </div>
  </div>
<?php endif; ?>
