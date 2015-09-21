
/**
 * @file
 * Attaches the behaviors for the Nivo Slider module.
 */

(function ($) {
  Backdrop.behaviors.nivoSlider = {
    attach: function (context, settings) {
      $('.nivoSlider').each(function(){
        sliderId = $(this).attr("id");
        slider = Backdrop.settings.sliderSettings[sliderId];
        
        // Initialize the slider
        $(this).nivoSlider({
          'effect': slider.effect, // Specify sets like: 'fold,fade,sliceDown'
          'slices': slider.slices, // For slice animations
          'boxCols': slider.boxCols, // For box animations
          'boxRows': slider.boxRows, // For box animations
          'animSpeed': slider.animSpeed, // Slide transition speed
          'pauseTime': slider.pauseTime, // How long each slide will show
          'startSlide': slider.startSlide, // Set starting Slide (0 index)
          'directionNav': slider.directionNav, // Next & Prev navigation
          'directionNavHide': slider.directionNavHide, // Only show on hover
          'controlNav': slider.controlNav, // 1,2,3... navigation
          'controlNavThumbs': slider.controlNavThumbs, // Use thumbnails for Control Nav
          'pauseOnHover': slider.pauseOnHover, // Stop animation while hovering
          'manualAdvance': slider.manualAdvance, // Force manual transitions
          'prevText': slider.prevText, // Prev directionNav text
          'nextText': slider.nextText, // Next directionNav text
          'randomStart': slider.randomStart, // Start on a random slide
          'beforeChange': slider.beforeChange, // Triggers before a slide transition
          'afterChange': slider.afterChange, // Triggers after a slide transition
          'slideshowEnd': slider.slideshowEnd, // Triggers after all slides have been shown
          'lastSlide': slider.lastSlide, // Triggers when last slide is shown
          'afterLoad': slider.afterLoad // Triggers when slider has loaded
        });
      });
      
    }
  };
}(jQuery));
