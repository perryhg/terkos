//======================================================================================================================
// Class for managing common dialogs.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
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
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.DialogManager.js";
   alert(noJQuery);
   throw new Error(noJQuery);
   }
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================
(function()
   {
   // ==================================================================================================================
   var jQuery = window['$'];

   edu.cmu.ri.terk.DialogManager = function(
         nonClosableWaitDialog,
         nonClosableWaitDialogMessageId,
         okDialogId,
         okDialogMessageId,
         okDialogOkButtonId)
      {
      jQuery("#" + okDialogId).dialog({
         bgiframe: true,
         autoOpen: false,
         modal: true,
         draggable: false,
         resizable: false});

      jQuery("#" + okDialogOkButtonId).click(function()
         {
         jQuery("#" + okDialogId).dialog('close');
         });

      jQuery("#" + nonClosableWaitDialog).dialog({
         bgiframe: true,
         autoOpen: false,
         modal: true,
         draggable: false,
         resizable: false,
         closeOnEscape : false,
         title : 'Saving Preferences',
         dialogClass: 'non_closable_alert'});

      var showDialog = function(dialogId, dialogMessageId, message)
         {
         jQuery("#" + dialogMessageId).html(message);

         // Show the dialog and set the min-height.  I don't know why, but I *had* to set the min-height
         // here...setting it in the HTML or on load didn't work--it kept getting overwritten to
         // something like 112 or 114 pixels.
         jQuery("#" + dialogId).dialog('open').css('min-height', '1px');
         };

      var hideDialog = function(dialogId)
         {
         jQuery("#" + dialogId).dialog('close');
         };

      this.showOKDialog = function(message)
         {
         showDialog(okDialogId, okDialogMessageId, message);
         };

      this.hideOKDialog = function()
         {
         hideDialog(okDialogId);
         };

      this.showNonClosableWaitDialog = function(message)
         {
         showDialog(nonClosableWaitDialog, nonClosableWaitDialogMessageId, message);
         };

      this.hideNonClosableWaitDialog = function()
         {
         hideDialog(nonClosableWaitDialog);
         };

      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();

