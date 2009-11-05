//======================================================================================================================
// Class for initializing and configuring the various widgets on the Audio tab.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
// * jquery.mousecapture.js (http://benanne.net/code/?p=238)
// * edu.cmu.ri.terk.NonClosableWaitDialog
// * edu.cmu.ri.terk.SingleButtonModalDialog
// * main.css
//
// Author: Chris Bartley (bartley@cmu.edu)
//======================================================================================================================

//======================================================================================================================
// VERIFY NAMESPACE
//======================================================================================================================
// Create the global symbol "edu" if it doesn't exist.  Throw an error if it does exist but is not an object.
var edu;
if (!edu)
   {
   edu = {};
   }
else if (typeof edu != "object")
   {
   var eduExistsMessage = "Error: failed to create edu namespace: edu already exists and is not an object";
   alert(eduExistsMessage);
   throw new Error(eduExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu)
   {
   edu.cmu = {};
   }
else if (typeof edu.cmu != "object")
   {
   var eduCmuExistsMessage = "Error: failed to create edu.cmu namespace: edu.cmu already exists and is not an object";
   alert(eduCmuExistsMessage);
   throw new Error(eduCmuExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri)
   {
   edu.cmu.ri = {};
   }
else if (typeof edu.cmu.ri != "object")
   {
   var eduCmuRiExistsMessage = "Error: failed to create edu.cmu.ri namespace: edu.cmu.ri already exists and is not an object";
   alert(eduCmuRiExistsMessage);
   throw new Error(eduCmuRiExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri.terk)
   {
   edu.cmu.ri.terk = {};
   }
else if (typeof edu.cmu.ri.terk != "object")
   {
   var eduCmuRiTerkExistsMessage = "Error: failed to create edu.cmu.ri.terk namespace: edu.cmu.ri.terk already exists and is not an object";
   alert(eduCmuRiTerkExistsMessage);
   throw new Error(eduCmuRiTerkExistsMessage);
   }
//======================================================================================================================

//======================================================================================================================
// DEPENDECIES
//======================================================================================================================
if (!window['$'])
   {
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.AudioTab.js";
   alert(noJQuery);
   throw new Error(noJQuery);
   }
//======================================================================================================================
if (!edu.cmu.ri.terk.NonClosableWaitDialog)
   {
   var noNonClosableWaitDialog = "The edu.cmu.ri.terk.NonClosableWaitDialog library is required by edu.cmu.ri.terk.AudioTab.js";
   alert(noNonClosableWaitDialog);
   throw new Error(noNonClosableWaitDialog);
   }
//======================================================================================================================
if (!edu.cmu.ri.terk.SingleButtonModalDialog)
   {
   var noSingleButtonModalDialog = "The edu.cmu.ri.terk.SingleButtonModalDialog library is required by edu.cmu.ri.terk.AudioTab.js";
   alert(noSingleButtonModalDialog);
   throw new Error(noSingleButtonModalDialog);
   }
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================
(function()
   {
   // ==================================================================================================================
   var jQuery = window['$'];

   edu.cmu.ri.terk.AudioTab = function(audioConfigManager)
      {
      jQuery("#volumeSlider")['slider']({
         animate: true,
         range: "min",
         value: 5,
         min: 0,
         max: 10,
         step: 1,
         slide: function(event, ui)
            {
            jQuery("#volume").text(ui.value);
            audioConfigManager.setVolume(ui.value);
            },
         stop: function(event, ui)
            {
               //alert(event.or)
            audioConfigManager.playSampleSound(ui.value);
            }
      });

      // configure the checkbox
      jQuery('#isAudioAlertsEnabled')['change'](
            function()
               {
               audioConfigManager.setAudioAlertsEnabled(jQuery(this).attr("checked"));
               }
            );

      // add mouse event handlers to the Save button
      jQuery('#saveAudioConfigButton').mousecapture({
         "down": function()
            {
            if (audioConfigManager.isModified())
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (audioConfigManager.isModified())
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }}).click(
            function()
               {
               if (audioConfigManager.isModified())
                  {
                  audioConfigManager.saveAudioConfig();
                  }
               }
            ).disableSelection();

      var errorOccurredDialog = new edu.cmu.ri.terk.SingleButtonModalDialog(
            "Error",
            "Sorry, an error occurred while saving your preferences.  Please try again.",
            "OK");
      errorOccurredDialog.addEventListener({
         "onButtonClick": function()
            {
            errorOccurredDialog.hide();
            }
      });

      var nonClosableWaitDialog = new edu.cmu.ri.terk.NonClosableWaitDialog(
            "Saving Preferences",
            "Please wait while your changes to the audio configuration are being saved.");

      // register the event listener
      audioConfigManager.addEventListener({
         onBeforeLoad : function()
            {
            jQuery("#audioConfigurationMessageArea").html("Loading preferences...");
            jQuery("#audioConfigurationArea").hide();
            },
         onLoadSuccess : function()
            {
            var isAlertsEnabled = audioConfigManager.isAudioAlertsEnabled();
            var volume = audioConfigManager.getVolume();
            jQuery('#isAudioAlertsEnabled').attr('checked', isAlertsEnabled);
            jQuery("#volumeSlider")['slider']('value', volume);
            jQuery("#volume").text(volume);
            jQuery("#audioConfigurationMessageArea").empty();
            jQuery("#audioConfigurationArea").show();
            },
         onLoadFailure : function()
            {
            jQuery("#audioConfigurationMessageArea").html("Sorry, the audio configuration is currently unavailable.");
            },
         isModified : function(isModified)
            {
            // toggle the state of the save button according to whether the config has been modified
            jQuery("#saveAudioConfigButton").toggleClass("ui-state-default", isModified);
            jQuery("#saveAudioConfigButton").toggleClass("ui-state-active", !isModified);
            jQuery("#saveAudioConfigButton").toggleClass("ui-state-disabled", !isModified);
            },
         onBeforeSave: function()
            {
            nonClosableWaitDialog.show();
            },
         onSaveSuccess: function()
            {
            nonClosableWaitDialog.hide();
            },
         onSaveFailure: function()
            {
            nonClosableWaitDialog.hide();
            errorOccurredDialog.show();
            }});

      this.activate = function()
         {
         audioConfigManager.getAudioConfig();
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();

         