//======================================================================================================================
// Class for initializing and configuring the various widgets on the Wireless Networking tab.
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
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.WirelessNetworkingTab.js";
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

   edu.cmu.ri.terk.WirelessNetworkingTab = function(wirelessNetworkingConfigManager, wirelessNetworkScanner)
      {
      var isNetworkNameValid = function(formFieldID)
         {
         return jQuery(formFieldID).val().length > 0;
         };

      var showEditPreferredNetworkDialog = function()
         {
         if (wirelessNetworkingConfigManager.isItemSelected())
            {
            var selectedItemJSON = wirelessNetworkingConfigManager.getSelectedItemJSON();
            jQuery("#wirelessNetworkName_edit").val(selectedItemJSON['ssid']);
            jQuery("#wirelessNetworkUUID_edit").val(wirelessNetworkingConfigManager.getSelectedItemUUID());
            jQuery("#wirelessNetworkName_edit").keyup();
            jQuery('#edit-preferred-wireless-network-dialog').dialog('open');
            }
         };

      // configure the WirelessNetworkScanner
      wirelessNetworkScanner.addSelectionListener(function(isSelected)
         {
         jQuery(".state-depends-on-available-wireless-network-list-selection").toggleClass("ui-state-default", isSelected);
         jQuery(".state-depends-on-available-wireless-network-list-selection").toggleClass("ui-state-active", !isSelected);
         jQuery(".state-depends-on-available-wireless-network-list-selection").toggleClass("ui-state-disabled", !isSelected);
         });

      // configure the WirelessNetworkingConfigManager
      wirelessNetworkingConfigManager.addSelectionListener(function(isSelected)
         {
         jQuery(".state-depends-on-preferred-wireless-network-list-selection").toggleClass("ui-state-default", isSelected);
         jQuery(".state-depends-on-preferred-wireless-network-list-selection").toggleClass("ui-state-active", !isSelected);
         jQuery(".state-depends-on-preferred-wireless-network-list-selection").toggleClass("ui-state-disabled", !isSelected);
         });
      wirelessNetworkingConfigManager.addDoubleClickListener(showEditPreferredNetworkDialog);
      wirelessNetworkingConfigManager.addChangeListener(
            function(isModified)
               {
               jQuery("#saveWirelessNetworkingConfigButton").toggleClass("ui-state-default", isModified);
               jQuery("#saveWirelessNetworkingConfigButton").toggleClass("ui-state-active", !isModified);
               jQuery("#saveWirelessNetworkingConfigButton").toggleClass("ui-state-disabled", !isModified);
               }
            );

      // ---------------------------------------------------------------------------------------------------------------
      // Common configuration
      // ---------------------------------------------------------------------------------------------------------------

      jQuery('.state-has-no-dependency').mousecapture({
         "down": function()
            {
            jQuery(this).addClass('ui-state-active');
            },
         "up": function()
            {
            jQuery(this).removeClass('ui-state-active');
            }
      });

      jQuery('.cancel-preferred-wireless-network-dialog-button').click(
            function()
               {
               jQuery("#preferred-wireless-network-dialog").dialog('close');
               }
            );

      // ---------------------------------------------------------------------------------------------------------------
      // Form for managing preferred wireless networks
      // ---------------------------------------------------------------------------------------------------------------

      jQuery('#willStartWirelessNetworkingOnBootupCheckbox').change(
            function()
               {
               wirelessNetworkingConfigManager.setWillStartOnBootup(jQuery(this).attr("checked"));
               }
            );

      // add mouse event handlers to the Add Preferred Wireless Network button
      jQuery('#addPreferredWirelessNetworkButton').click(
            function()
               {
               jQuery('#preferred-wireless-network-dialog').dialog('open');
               }
            ).disableSelection();

      // add mouse event handlers to the Delete Preferred Wireless Network button
      jQuery('#deletePreferredWirelessNetworkButton').click(
            function()
               {
               wirelessNetworkingConfigManager.deleteSelected();
               }
            ).disableSelection();

      // add mouse event handlers to the Edit Preferred Wireless Network button
      jQuery('#editPreferredWirelessNetworkButton').click(
            function()
               {
               showEditPreferredNetworkDialog();
               }
            ).disableSelection();

      // add mousedown/mouseup event handlers to the Delete and Edit preferred wireless network buttons
      jQuery('.state-depends-on-preferred-wireless-network-list-selection').mousecapture({
         "down": function()
            {
            if (wirelessNetworkingConfigManager.isItemSelected())
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (wirelessNetworkingConfigManager.isItemSelected())
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }});

      // add mouse event handlers to the Save button
      jQuery('#saveWirelessNetworkingConfigButton').mousecapture({
         "down": function()
            {
            if (wirelessNetworkingConfigManager.isModified())
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (wirelessNetworkingConfigManager.isModified())
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }}).click(
            function()
               {
               if (wirelessNetworkingConfigManager.isModified())
                  {
                  alert("Save not implemented!");
                  }
               }
            ).disableSelection();

      // ---------------------------------------------------------------------------------------------------------------
      // Dialog for manually adding a preferred wireless network
      // ---------------------------------------------------------------------------------------------------------------

      // create the dialog for entering/specifying a preferred wireless network
      var preferredWirelessNetworkDialogFields = jQuery([]).add(jQuery("#wirelessNetworkName_add"));
      jQuery("#preferred-wireless-network-dialog").dialog({
         bgiframe: true,
         autoOpen: false,
         modal: true,
         draggable: false,
         resizable: false,
         close: function()
            {
            preferredWirelessNetworkDialogFields.val('');
            jQuery('#manuallyEnterAWirelessNetworkButton').click();
            jQuery("#wirelessNetworkName_add").keyup();
            }
      });

      jQuery("#wirelessNetworkName_add").keyup(
            function()
               {
               var isValid = isNetworkNameValid("#wirelessNetworkName_add");
               jQuery("#addManuallyEnteredWirelessNetworkDialogButton").toggleClass("ui-state-default", isValid);
               jQuery("#addManuallyEnteredWirelessNetworkDialogButton").toggleClass("ui-state-active", !isValid);
               jQuery("#addManuallyEnteredWirelessNetworkDialogButton").toggleClass("ui-state-disabled", !isValid);
               }
            );

      jQuery('#addManuallyEnteredWirelessNetworkDialogButton').mousecapture({
         "down": function()
            {
            if (isNetworkNameValid("#wirelessNetworkName_add"))
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (isNetworkNameValid("#wirelessNetworkName_add"))
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }
      }).click(
            function()
               {
               if (isNetworkNameValid("#wirelessNetworkName_add"))
                  {
                  // build the network profile object
                  var network = new Object();
                  network['ssid'] = jQuery("#wirelessNetworkName_add").val();
                  network['is-encrypted'] = false;

                  // add the network
                  wirelessNetworkingConfigManager.addNetwork(network);

                  // close the dialog
                  jQuery("#preferred-wireless-network-dialog").dialog('close');
                  }
               }
            );

      jQuery('#showAvailableWirelessNetworksButton').click(
            function()
               {
               jQuery("#formToEnterPreferredWirelessNetworkInfo").addClass("hidden");
               jQuery("#formToChooseFromAvailableWirelessNetworks").removeClass("hidden");

               wirelessNetworkScanner.scanForWirelessNetworks();
               }
            );

      // ---------------------------------------------------------------------------------------------------------------
      // Dialog for selecting an existing wireless network
      // ---------------------------------------------------------------------------------------------------------------

      jQuery('#manuallyEnterAWirelessNetworkButton').click(
            function()
               {
               jQuery("#formToEnterPreferredWirelessNetworkInfo").removeClass("hidden");
               jQuery("#formToChooseFromAvailableWirelessNetworks").addClass("hidden");
               }
            );

      // add mouse event handlers to the Add available wireless network button
      jQuery('#addAvailableWirelessNetworkDialogButton').mousecapture({
         "down": function()
            {
            if (wirelessNetworkScanner.isItemSelected())
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (wirelessNetworkScanner.isItemSelected())
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }
      }).click(
            function()
               {
               if (wirelessNetworkScanner.isItemSelected())
                  {
                  var json = wirelessNetworkScanner.getSelectedItemJSON();

                  // build a copy of network profile object, containing only the bits we care about.
                  var network = new Object();
                  network['ssid'] = json['ssid'];
                  network['is-encrypted'] = json['is-encrypted'];

                  // add the network
                  wirelessNetworkingConfigManager.addNetwork(network);

                  // close the dialog
                  jQuery("#preferred-wireless-network-dialog").dialog('close');
                  }
               }
            );

      // ---------------------------------------------------------------------------------------------------------------
      // Dialog for editing a preferred wireless network
      // ---------------------------------------------------------------------------------------------------------------

      // create the dialog for editing a preferred wireless network
      var editWirelessNetworkDialogFields = jQuery([]).add(jQuery("#wirelessNetworkName_edit"));
      jQuery("#edit-preferred-wireless-network-dialog").dialog({
         bgiframe: true,
         autoOpen: false,
         modal: true,
         draggable: false,
         resizable: false,
         close: function()
            {
            editWirelessNetworkDialogFields.val('');
            jQuery("#wirelessNetworkName_edit").keyup();
            }
      });

      jQuery("#wirelessNetworkName_edit").keyup(
            function()
               {
               var isValid = isNetworkNameValid("#wirelessNetworkName_edit");
               jQuery("#saveManuallyEnteredWirelessNetworkDialogButton").toggleClass("ui-state-default", isValid);
               jQuery("#saveManuallyEnteredWirelessNetworkDialogButton").toggleClass("ui-state-active", !isValid);
               jQuery("#saveManuallyEnteredWirelessNetworkDialogButton").toggleClass("ui-state-disabled", !isValid);
               }
            );

      jQuery('#saveManuallyEnteredWirelessNetworkDialogButton').mousecapture({
         "down": function()
            {
            if (isNetworkNameValid("#wirelessNetworkName_edit"))
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (isNetworkNameValid("#wirelessNetworkName_edit"))
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }
      }).click(
            function()
               {
               if (isNetworkNameValid("#wirelessNetworkName_edit"))
                  {
                  // build the network profile object
                  var network = new Object();
                  network['ssid'] = jQuery("#wirelessNetworkName_edit").val();
                  network['is-encrypted'] = false;

                  // update the config
                  wirelessNetworkingConfigManager.editNetwork(jQuery("#wirelessNetworkUUID_edit").val(), network);

                  // close the dialog
                  jQuery("#edit-preferred-wireless-network-dialog").dialog('close');
                  }
               }
            );

      jQuery('#cancelEditWirelessNetworkDialogButton').click(
            function()
               {
               jQuery("#edit-preferred-wireless-network-dialog").dialog('close');
               }
            );

      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();


      