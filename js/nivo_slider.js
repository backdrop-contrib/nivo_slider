
/**
 * @file
 * Attaches the behaviors for the Nivo Slider module.
 */

(function ($) {
  Backdrop.behaviors.nivoSlider = {
    attach: function (context, settings) {
      // Initialize the slider
      $('#slider').nivoSlider({
        'effect': Backdrop.settings.nivo_slider.effect, // Specify sets like: 'fold,fade,sliceDown'
        'slices': Backdrop.settings.nivo_slider.slices, // For slice animations
        'boxCols': Backdrop.settings.nivo_slider.boxCols, // For box animations
        'boxRows': Backdrop.settings.nivo_slider.boxRows, // For box animations
        'animSpeed': Backdrop.settings.nivo_slider.animSpeed, // Slide transition speed
        'pauseTime': Backdrop.settings.nivo_slider.pauseTime, // How long each slide will show
        'startSlide': Backdrop.settings.nivo_slider.startSlide, // Set starting Slide (0 index)
        'directionNav': Backdrop.settings.nivo_slider.directionNav, // Next & Prev navigation
        'directionNavHide': Backdrop.settings.nivo_slider.directionNavHide, // Only show on hover
        'controlNav': Backdrop.settings.nivo_slider.controlNav, // 1,2,3... navigation
        'controlNavThumbs': Backdrop.settings.nivo_slider.controlNavThumbs, // Use thumbnails for Control Nav
        'pauseOnHover': Backdrop.settings.nivo_slider.pauseOnHover, // Stop animation while hovering
        'manualAdvance': Backdrop.settings.nivo_slider.manualAdvance, // Force manual transitions
        'prevText': Backdrop.settings.nivo_slider.prevText, // Prev directionNav text
        'nextText': Backdrop.settings.nivo_slider.nextText, // Next directionNav text
        'randomStart': Backdrop.settings.nivo_slider.randomStart, // Start on a random slide
        'beforeChange': Backdrop.settings.nivo_slider.beforeChange, // Triggers before a slide transition
        'afterChange': Backdrop.settings.nivo_slider.afterChange, // Triggers after a slide transition
        'slideshowEnd': Backdrop.settings.nivo_slider.slideshowEnd, // Triggers after all slides have been shown
        'lastSlide': Backdrop.settings.nivo_slider.lastSlide, // Triggers when last slide is shown
        'afterLoad': Backdrop.settings.nivo_slider.afterLoad // Triggers when slider has loaded
      });
    }
  };
}(jQuery));
