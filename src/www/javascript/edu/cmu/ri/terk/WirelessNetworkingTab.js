//======================================================================================================================
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the GNU General Public License v2
// (http://www.gnu.org/licenses/gpl-2.0.html). Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at telepresencerobotkit@cs.cmu.edu. Such licensing
// terms are available for all portions of the Terk and TerkOS codebase presented here.
//======================================================================================================================
// Class for initializing and configuring the various widgets on the Wireless Networking tab.
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
else
   {
   if (typeof edu != "object")
      {
      var eduExistsMessage = "Error: failed to create edu namespace: edu already exists and is not an object";
      alert(eduExistsMessage);
      throw new Error(eduExistsMessage);
      }
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu)
   {
   edu.cmu = {};
   }
else
   {
   if (typeof edu.cmu != "object")
      {
      var eduCmuExistsMessage = "Error: failed to create edu.cmu namespace: edu.cmu already exists and is not an object";
      alert(eduCmuExistsMessage);
      throw new Error(eduCmuExistsMessage);
      }
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri)
   {
   edu.cmu.ri = {};
   }
else
   {
   if (typeof edu.cmu.ri != "object")
      {
      var eduCmuRiExistsMessage = "Error: failed to create edu.cmu.ri namespace: edu.cmu.ri already exists and is not an object";
      alert(eduCmuRiExistsMessage);
      throw new Error(eduCmuRiExistsMessage);
      }
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri.terk)
   {
   edu.cmu.ri.terk = {};
   }
else
   {
   if (typeof edu.cmu.ri.terk != "object")
      {
      var eduCmuRiTerkExistsMessage = "Error: failed to create edu.cmu.ri.terk namespace: edu.cmu.ri.terk already exists and is not an object";
      alert(eduCmuRiTerkExistsMessage);
      throw new Error(eduCmuRiTerkExistsMessage);
      }
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
if (!edu.cmu.ri.terk.NonClosableWaitDialog)
   {
   var noNonClosableWaitDialog = "The edu.cmu.ri.terk.NonClosableWaitDialog library is required by edu.cmu.ri.terk.WirelessNetworkingTab.js";
   alert(noNonClosableWaitDialog);
   throw new Error(noNonClosableWaitDialog);
   }
//======================================================================================================================
if (!edu.cmu.ri.terk.SingleButtonModalDialog)
   {
   var noSingleButtonModalDialog = "The edu.cmu.ri.terk.SingleButtonModalDialog library is required by edu.cmu.ri.terk.WirelessNetworkingTab.js";
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
   jQuery.ajaxSetup({
      type: 'POST',
      dataType: 'jsonp',
      timeout: 10000,
      cache: false,
      global: false
   });

   edu.cmu.ri.terk.WirelessNetworkingTab = function(wirelessNetworkingConfigManager,
                                                    wirelessNetworkScanner,
                                                    wirelessNetworkingStatus,
                                                    host)
      {
      var isFormFieldNonEmpty = function(formFieldID)
         {
         return jQuery(formFieldID).val().length > 0;
         };

      var isFormFieldLengthBetween = function(formFieldID, minLength, maxLength)
         {
         var length = jQuery(formFieldID).val().length;
         return length >= minLength && length <= maxLength;
         };

      var isHexTextFieldValue = function(formFieldID, length)
         {
         return jQuery(formFieldID).val().length == length && jQuery(formFieldID).val().match(/^[a-fA-F0-9]+$/);
         };

      var showEditPreferredNetworkDialog = function()
         {
         if (wirelessNetworkingConfigManager.isItemSelected())
            {
            var selectedItemJSON = wirelessNetworkingConfigManager.getSelectedItemJSON();
            jQuery("#wirelessNetworkName_edit").val(selectedItemJSON['ssid']);
            jQuery("#wirelessNetworkUUID_edit").val(wirelessNetworkingConfigManager.getSelectedItemUUID());
            jQuery("#wirelessNetworkName_edit").keyup();
            jQuery('#editPreferredWirelessNetworkDialog').dialog('open');
            }
         };

      var setWidgetEnabled = function(idOrClass, isEnabled)
         {
         if (isEnabled)
            {
            jQuery(idOrClass).addClass("ui-state-default");
            jQuery(idOrClass).removeClass("ui-state-active");
            jQuery(idOrClass).removeClass("ui-state-disabled");
            }
         else
            {
            jQuery(idOrClass).removeClass("ui-state-default");
            jQuery(idOrClass).addClass("ui-state-active");
            jQuery(idOrClass).addClass("ui-state-disabled");
            }
         };

      // configure the WirelessNetworkScanner
      wirelessNetworkScanner.addSelectionListener(function()
         {
         // clear the fields so that no previously-entered info appears
         jQuery("#availableWEPNetworkPassword").val("");
         jQuery("#availableWEPNetworkPasswordLength").get(0).selectedIndex = 0;
         jQuery("#availableWEPNetworkForm").hide();
         jQuery("#availableWPANetworkPassword").val("");
         jQuery("#availableWPANetworkPasswordType").get(0).selectedIndex = 0;
         jQuery("#availableWPANetworkForm").hide();

         // if the network is encrypted, then show the password form, otherwise hide the password form and enable the Add button
         var json = wirelessNetworkScanner.getSelectedItemJSON();
         if (json && json['is-encrypted'])
            {
            // disable the add button
            setWidgetEnabled("#addAvailableWirelessNetworkDialogButton", false);

            if (json['encryption-type'] == "wpa")
               {
               jQuery("#availableWPANetworkForm").show();
               }
            else
               {
               jQuery("#availableWEPNetworkForm").show();
               }
            }
         else
            {
            // enable the add button
            setWidgetEnabled("#addAvailableWirelessNetworkDialogButton", true);
            }
         });

      // configure the WirelessNetworkingConfigManager
      wirelessNetworkingConfigManager.addSelectionListener(function(isSelected)
         {
         setWidgetEnabled(".state-depends-on-preferred-wireless-network-list-selection", isSelected);
         });
      wirelessNetworkingConfigManager.addDoubleClickListener(showEditPreferredNetworkDialog);
      wirelessNetworkingConfigManager.addChangeListener(
            function(isModified)
               {
               // toggle the state of the save button according to whether the config has been modified
               setWidgetEnabled("#saveWirelessNetworkingConfigButton", isModified);
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
               jQuery("#preferredWirelessNetworkDialog").dialog('close');
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
               jQuery('#preferredWirelessNetworkDialog').dialog('open');
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
            "Please wait while your changes to the wireless networking configuration are being saved.");

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
                  nonClosableWaitDialog.show();

                  // get the JSON to send to the backend
                  var json = wirelessNetworkingConfigManager.getJSON();

                  // send the JSON to the backend
                  jQuery.ajax(
                  {
                     url: host + '/cgi-bin/saveWirelessNetworkingConfig.pl',
                     data: "json=" + json,
                     success: function(jsonResponse)
                        {
                        if (jsonResponse)
                           {
                           // reload the config
                           wirelessNetworkingConfigManager.loadWirelessNetworkingConfig(function()
                              {
                              nonClosableWaitDialog.hide();
                              });
                           }
                        else
                           {
                           nonClosableWaitDialog.hide();
                           errorOccurredDialog.show();
                           }
                        },
                     error: function()
                        {
                        nonClosableWaitDialog.hide();
                        errorOccurredDialog.show();
                        }
                  });
                  }
               }
            ).disableSelection();
      // ---------------------------------------------------------------------------------------------------------------
      // Dialog for manually adding a preferred wireless network
      // ---------------------------------------------------------------------------------------------------------------

      // create the dialog for entering/specifying a preferred wireless network
      var preferredWirelessNetworkDialogFields = jQuery([]).add(jQuery("#wirelessNetworkName_add"),
                                                                jQuery("#preferredWEPNetworkPassword_add"),
                                                                jQuery("#preferredWPANetworkPassword_add"));
      jQuery("#preferredWirelessNetworkDialog").dialog({
         bgiframe: true,
         autoOpen: false,
         modal: true,
         draggable: false,
         resizable: false,
         close: function()
            {
            jQuery("#wirelessNetworkName_add").val('');
            jQuery("#wirelessNetworkEncryptionType_add").get(0).selectedIndex = 0;

            jQuery(".preferredWirelessNetworkWEPSubForm_add").hide();
            jQuery("#preferredWEPNetworkPassword_add").val('');
            jQuery("#preferredWEPNetworkPasswordLength_add").get(0).selectedIndex = 0;
            jQuery("#preferredWEPNetworkPasswordHint_add").text("Please select the encryption strength");

            jQuery(".preferredWirelessNetworkWPASubForm_add").hide();
            jQuery("#preferredWPANetworkPassword_add").val('');
            jQuery("#preferredWPANetworkPasswordType_add").get(0).selectedIndex = 0;
            jQuery("#preferredWPANetworkPasswordHint_add").text("Please select the password type");

            jQuery('#manuallyEnterAWirelessNetworkButton').click();
            jQuery("#wirelessNetworkName_add").keyup();
            }
      });

      var isPreferredWirelessNetworkFormValid = function()
         {
         var isValid = false;
         var encryptionType = jQuery("#wirelessNetworkEncryptionType_add").val();
         if (encryptionType == "wep")
            {
            var requiredPasswordLength = jQuery("#preferredWEPNetworkPasswordLength_add").val();
            isValid = requiredPasswordLength > 0 && isHexTextFieldValue("#preferredWEPNetworkPassword_add", requiredPasswordLength);
            }
         else
            {
            if (encryptionType == "wpa")
               {
               // get the password type
               var passwordType = jQuery("#preferredWPANetworkPasswordType_add").val();

               if (passwordType == "text")
                  {
                  // password must be between 8 and 64 characters, inclusive
                  isValid = isFormFieldLengthBetween("#preferredWPANetworkPassword_add", 8, 63);
                  }
               else
                  {
                  if (passwordType == "hex")
                     {
                     // password must be 64 hex characters
                     isValid = isHexTextFieldValue("#preferredWPANetworkPassword_add", 64);
                     }
                  }
               }
            else
               {
               isValid = true;
               }
            }

         return isValid && isFormFieldNonEmpty("#wirelessNetworkName_add");
         };

      var validatePreferredWirelessNetworkForm = function()
         {
         setWidgetEnabled("#addManuallyEnteredWirelessNetworkDialogButton", isPreferredWirelessNetworkFormValid());
         };

      jQuery("#formToEnterPreferredWirelessNetworkInfo").submit(
            function()
               {
               if (isPreferredWirelessNetworkFormValid())
                  {
                  jQuery('#addManuallyEnteredWirelessNetworkDialogButton').click();
                  }
               return false;
               }
            );

      jQuery("#wirelessNetworkName_add").keyup(
            function()
               {
               validatePreferredWirelessNetworkForm();
               }
            );

      jQuery("#wirelessNetworkEncryptionType_add").change(
            function()
               {
               jQuery(".preferredWirelessNetworkWEPSubForm_add").hide();
               jQuery(".preferredWirelessNetworkWPASubForm_add").hide();

               var encryptionType = jQuery("#wirelessNetworkEncryptionType_add").val();
               if (encryptionType == "wep")
                  {
                  jQuery(".preferredWirelessNetworkWEPSubForm_add").show();
                  }
               else
                  {
                  if (encryptionType == "wpa")
                     {
                     jQuery(".preferredWirelessNetworkWPASubForm_add").show();
                     }
                  }
               validatePreferredWirelessNetworkForm();
               }
            );

      jQuery("#preferredWEPNetworkPassword_add").keyup(
            function()
               {
               validatePreferredWirelessNetworkForm();
               }
            );

      jQuery("#preferredWPANetworkPassword_add").keyup(
            function()
               {
               validatePreferredWirelessNetworkForm();
               }
            );

      jQuery("#preferredWEPNetworkPasswordLength_add").change(
            function()
               {
               var requiredPasswordLength = jQuery("#preferredWEPNetworkPasswordLength_add").val();
               var hint = "Please select the encryption strength";
               if (requiredPasswordLength > 0)
                  {
                  hint = "Password must be " + requiredPasswordLength + " hex digits";
                  }
               jQuery("#preferredWEPNetworkPasswordHint_add").text(hint);
               validatePreferredWirelessNetworkForm();
               }
            );

      jQuery("#preferredWPANetworkPasswordType_add").change(
            function()
               {
               var passwordType = jQuery("#preferredWPANetworkPasswordType_add").val();
               var hint = "Please select the password type";
               if (passwordType == "text")
                  {
                  hint = "Password must be 8 - 63 characters";
                  }
               else
                  {
                  if (passwordType == "hex")
                     {
                     hint = "Password must be 64 hex digits";
                     }
                  }
               jQuery("#preferredWPANetworkPasswordHint_add").text(hint);
               validatePreferredWirelessNetworkForm();
               }
            );

      jQuery('#addManuallyEnteredWirelessNetworkDialogButton').mousecapture({
         "down": function()
            {
            if (isPreferredWirelessNetworkFormValid())
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (isPreferredWirelessNetworkFormValid())
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }
      }).click(
            function()
               {
               if (isPreferredWirelessNetworkFormValid())
                  {
                  // build the network profile object
                  var network = new Object();
                  network['ssid'] = jQuery("#wirelessNetworkName_add").val();
                  network['is-encrypted'] = false;
                  network['encryption-type'] = "none";
                  // TODO!!!!

                  // add the network
                  wirelessNetworkingConfigManager.addNetwork(network);

                  // close the dialog
                  jQuery("#preferredWirelessNetworkDialog").dialog('close');
                  }
               }
            );

      jQuery('#showAvailableWirelessNetworksButton').click(
            function()
               {
               jQuery("#formToEnterPreferredWirelessNetworkInfo").hide();
               jQuery("#formToChooseFromAvailableWirelessNetworks").show();

               wirelessNetworkScanner.scanForWirelessNetworks();
               }
            );

      // ---------------------------------------------------------------------------------------------------------------
      // Dialog for selecting an existing wireless network
      // ---------------------------------------------------------------------------------------------------------------

      jQuery('#manuallyEnterAWirelessNetworkButton').click(
            function()
               {
               jQuery("#formToEnterPreferredWirelessNetworkInfo").show();
               jQuery("#formToChooseFromAvailableWirelessNetworks").hide();
               }
            );

      // add mouse event handlers to the Add available wireless network button
      jQuery('#addAvailableWirelessNetworkDialogButton').mousecapture({
         "down": function()
            {
            if (isAvailableNetworkFormValid())
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (isAvailableNetworkFormValid())
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }
      }).click(
            function()
               {
               if (isAvailableNetworkFormValid())
                  {
                  var json = wirelessNetworkScanner.getSelectedItemJSON();

                  // build a copy of network profile object, containing only the bits we care about.
                  var network = new Object();
                  network['ssid'] = json['ssid'];
                  network['is-encrypted'] = json['is-encrypted'];
                  network['encryption-type'] = json['encryption-type'];
                  if (json['encryption-type'] == "wep")
                     {
                     network['password'] = jQuery("#availableWEPNetworkPassword").val();
                     network['is-hex-password'] = true;
                     }
                  else
                     {
                     if (json['encryption-type'] == "wpa")
                        {
                        network['password'] = jQuery("#availableWPANetworkPassword").val();
                        network['is-hex-password'] = jQuery("#availableWPANetworkPasswordType").val() == "hex";
                        }
                     }

                  // add the network
                  wirelessNetworkingConfigManager.addNetwork(network);

                  // close the dialog
                  jQuery("#preferredWirelessNetworkDialog").dialog('close');
                  }
               }
            );

      var isAvailableNetworkFormValid = function()
         {
         var json = wirelessNetworkScanner.getSelectedItemJSON();

         var isValid = false;
         if (json && json['is-encrypted'])
            {
            if (json['encryption-type'] == "wep")
               {
               var requiredPasswordLength = jQuery("#availableWEPNetworkPasswordLength").val();
               isValid = requiredPasswordLength > 0 && isHexTextFieldValue("#availableWEPNetworkPassword", requiredPasswordLength);
               }
            else
               {
               if (json['encryption-type'] == "wpa")
                  {
                  // get the password type
                  var passwordType = jQuery("#availableWPANetworkPasswordType").val();

                  if (passwordType == "text")
                     {
                     // password must be between 8 and 64 characters, inclusive
                     isValid = isFormFieldLengthBetween("#availableWPANetworkPassword", 8, 63);
                     }
                  else
                     {
                     if (passwordType == "hex")
                        {
                        // password must be 64 hex characters
                        isValid = isHexTextFieldValue("#availableWPANetworkPassword", 64);
                        }
                     }
                  }
               }
            }
         else
            {
            isValid = true;
            }

         return isValid;
         };

      var validateAvailableNetworkForm = function()
         {
         setWidgetEnabled("#addAvailableWirelessNetworkDialogButton", isAvailableNetworkFormValid());
         };

      jQuery("#availableWEPNetworkPassword").keyup(
            function()
               {
               validateAvailableNetworkForm();
               }
            );

      jQuery("#availableWPANetworkPassword").keyup(
            function()
               {
               validateAvailableNetworkForm();
               }
            );

      jQuery("#availableWEPNetworkPasswordLength").change(
            function()
               {
               var requiredPasswordLength = jQuery("#availableWEPNetworkPasswordLength").val();
               var hint = "Please select the encryption strength";
               if (requiredPasswordLength > 0)
                  {
                  hint = "Password must be " + requiredPasswordLength + " hex digits";
                  }
               jQuery("#availableWEPNetworkPasswordHint").text(hint);
               validateAvailableNetworkForm();
               }
            );

      jQuery("#availableWPANetworkPasswordType").change(
            function()
               {
               var passwordType = jQuery("#availableWPANetworkPasswordType").val();
               var hint = "Please select the password type";
               if (passwordType == "text")
                  {
                  hint = "Password must be 8 - 63 characters";
                  }
               else
                  {
                  if (passwordType == "hex")
                     {
                     hint = "Password must be 64 hex digits";
                     }
                  }
               jQuery("#availableWPANetworkPasswordHint").text(hint);
               validateAvailableNetworkForm();
               }
            );

      // ---------------------------------------------------------------------------------------------------------------
      // Dialog for editing a preferred wireless network
      // ---------------------------------------------------------------------------------------------------------------

      // create the dialog for editing a preferred wireless network
      var editWirelessNetworkDialogFields = jQuery([]).add(jQuery("#wirelessNetworkName_edit"));
      jQuery("#editPreferredWirelessNetworkDialog").dialog({
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

      jQuery("#formToEdiPreferredWirelessNetworkInfo").submit(
            function()
               {
               if (isFormFieldNonEmpty("#wirelessNetworkName_edit"))
                  {
                  jQuery('#saveManuallyEnteredWirelessNetworkDialogButton').click();
                  }
               return false;
               }
            );

      jQuery("#wirelessNetworkName_edit").keyup(
            function()
               {
               var isValid = isFormFieldNonEmpty("#wirelessNetworkName_edit");
               setWidgetEnabled("#saveManuallyEnteredWirelessNetworkDialogButton", isValid);
               }
            );

      jQuery('#saveManuallyEnteredWirelessNetworkDialogButton').mousecapture({
         "down": function()
            {
            if (isFormFieldNonEmpty("#wirelessNetworkName_edit"))
               {
               jQuery(this).addClass('ui-state-active');
               }
            },
         "up": function()
            {
            if (isFormFieldNonEmpty("#wirelessNetworkName_edit"))
               {
               jQuery(this).removeClass('ui-state-active');
               }
            }
      }).click(
            function()
               {
               if (isFormFieldNonEmpty("#wirelessNetworkName_edit"))
                  {
                  // build the network profile object
                  var network = new Object();
                  network['ssid'] = jQuery("#wirelessNetworkName_edit").val();
                  network['is-encrypted'] = false;
                  network['encryption-type'] = "none";

                  // update the config
                  wirelessNetworkingConfigManager.editNetwork(jQuery("#wirelessNetworkUUID_edit").val(), network);

                  // close the dialog
                  jQuery("#editPreferredWirelessNetworkDialog").dialog('close');
                  }
               }
            );

      jQuery('#cancelEditWirelessNetworkDialogButton').click(
            function()
               {
               jQuery("#editPreferredWirelessNetworkDialog").dialog('close');
               }
            );

      this.activate = function()
         {
         wirelessNetworkingConfigManager.getWirelessNetworkingConfig();
         wirelessNetworkingStatus.getWirelessNetworkingStatus();
         };
      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();


      