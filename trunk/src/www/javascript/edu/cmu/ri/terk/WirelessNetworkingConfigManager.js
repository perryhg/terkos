//======================================================================================================================
// Class for managing the wireless networking configuration.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
// * Math.uuid (http://www.broofa.com/blog/?p=151)
// * json2.js (http://www.JSON.org/json2.js)
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
   var msg1 = "The jQuery library is required by edu.cmu.ri.terk.WirelessNetworkingConfigManager.js";
   alert(msg1);
   throw new Error(msg1);
   }
//======================================================================================================================
if (!Math.uuid)
   {
   var msg2 = "The Math.uuid library is required by edu.cmu.ri.terk.WirelessNetworkingConfigManager.js";
   alert(msg2);
   throw new Error(msg2);
   }
//======================================================================================================================
if (!JSON)
   {
   var msg3 = "The json2.js library is required by edu.cmu.ri.terk.WirelessNetworkingConfigManager.js";
   alert(msg3);
   throw new Error(msg3);
   }
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================
(function()
   {
   var jQuery = window['$'];
   jQuery.ajaxSetup({
      type: 'GET',
      dataType: 'jsonp',
      timeout: 3000,
      cache: false,
      global: false
   });

   edu.cmu.ri.terk.WirelessNetworkingConfigManager = function(wirelessNetworkingConfigurationMessageAreaId,
                                                              wirelessNetworkingConfigurationAreaId,
                                                              willStartWirelessNetworkingOnBootupCheckboxId,
                                                              wirelessNetworksListContainerId,
                                                              wirelessNetworksListId,
                                                              host)
      {
      var selectedWirelessNetwork = null;
      var selectionListeners = new Array();
      var doubleClickListeners = new Array();
      var changeListeners = new Array();
      var stringifiedOriginalJSON = null;
      var json = null;
      var profilesMap = new Array();

      jQuery("#" + wirelessNetworksListId).sortable({
         handle : '.handle',
         axis: 'y',
         tolerance: 'pointer' ,
         containment: '#' + wirelessNetworksListContainerId,
         update : function ()
            {
            rebuildProfiles();
            }
      }).disableSelection();

      var rebuildProfiles = function()
         {
         // wipe the current profile ordering from the JSON
         json['profiles'] = new Array();

         // fetch the new ordering
         var desiredProfileOrdering = jQuery('#' + wirelessNetworksListId).sortable("toArray");

         // add the profiles to the JSON in the new order
         for (var i = 0; i < desiredProfileOrdering.length; i++)
            {
            var uuid = desiredProfileOrdering[i];
            json['profiles'][json['profiles'].length] = profilesMap[uuid];
            }

         notifyChangeListeners();
         };

      var getIsModified = function()
         {
         return (JSON.stringify(json) != stringifiedOriginalJSON);
         };
      this.isModified = function()
         {
         return getIsModified();
         };

      var notifyChangeListeners = function()
         {
         // TODO: for debugging only
         // jQuery("#info").html("<pre>" + JSON.stringify(json, null, "\t") + "</pre>");

         // compare stringified versions of the original JSON with the updated one to see if
         // anything has changed and then update the listeners accordingly
         var isModified = getIsModified();

         // notify listeners of the modification state
         jQuery.each(changeListeners, function(i, listener)
            {
            if (listener)
               {
               listener(isModified);
               }
            });
         };

      this.getWirelessNetworkingConfig = function()
         {
         jQuery("#" + wirelessNetworkingConfigurationMessageAreaId).html("Loading preferences...");
         jQuery("#" + wirelessNetworkingConfigurationAreaId).addClass("hidden");

         this.loadWirelessNetworkingConfig();
         };

      this.loadWirelessNetworkingConfig = function(onSuccessCallbackFunction)
         {
         // clear the list
         jQuery("#" + wirelessNetworksListId).empty();

         clearListSelection();

         // load the config
         jQuery.ajax(
         {
            url: host + '/cgi-bin/getWirelessNetworkingConfigAsJSON.pl',
            success: function(jsonResponse)
               {
               jQuery("#" + wirelessNetworkingConfigurationAreaId).removeClass("hidden");
               json = jsonResponse;
               stringifiedOriginalJSON = JSON.stringify(json);
               displayWirelessNetworkingConfig();

               if (onSuccessCallbackFunction)
                  {
                  onSuccessCallbackFunction();
                  }
               },
            error: function()
               {
               json = null;
               stringifiedOriginalJSON = null;
               displayWirelessNetworkingConfig();
               }
         });
         };

      var clearListSelection = function()
         {
         // reset the selected network
         selectedWirelessNetwork = null;

         // refresh the Preferred Networks list
         jQuery("#" + wirelessNetworksListId).sortable('refresh');

         // notify listeners that nothing is selected
         jQuery.each(selectionListeners, function(i, listener)
            {
            if (listener)
               {
               listener(false);
               }
            });
         };

      var createUUID = function()
         {
         return "profile_" + Math.uuid(10);
         };

      var addNetworkProfile = function(networkProfile)
         {
         if (networkProfile)
            {
            // create a UUID for this network, so we can uniquely reference it when the user
            // changes sort order and such
            var uuid = createUUID();

            // save the profile in the map for easy reference later
            profilesMap[uuid] = networkProfile;

            // build the list item
            var listItem = document.createElement("div");
            listItem.id = uuid;
            listItem.className = "sortable-list-item";

            var listItemInfo = document.createElement("div");
            listItemInfo.id = uuid + "_info";
            listItemInfo.className = "handle";

            // add the item to the list
            jQuery("#" + wirelessNetworksListId).append(listItem);
            jQuery("#" + listItem.id).append(listItemInfo);
            jQuery("#" + listItemInfo.id).text(networkProfile['ssid']);

            // add a mousedown event handler to the list item so we can keep
            // track of which one is selected (and also do some selection highlighting)
            jQuery("#" + listItem.id).mousedown(
                  function()
                     {
                     if (this != selectedWirelessNetwork)
                        {
                        // remove selection highlighting from the previously selected item, if any
                        if (selectedWirelessNetwork != null)
                           {
                           jQuery(selectedWirelessNetwork).removeClass("selected");
                           }
                        // add selection highlighting to the newly selected item
                        jQuery(this).addClass("selected");

                        // record which item is selected
                        selectedWirelessNetwork = this;

                        // notify listeners that something is now selected
                        jQuery.each(selectionListeners, function(i, listener)
                           {
                           if (listener)
                              {
                              listener(true);
                              }
                           });
                        }
                     }
                  ).dblclick(
                  function()
                     {
                     // notify listeners that the selected item was double-clicked
                     jQuery.each(doubleClickListeners, function(i, listener)
                        {
                        if (listener)
                           {
                           listener();
                           }
                        });
                     }
                  );
            }
         };

      var displayWirelessNetworkingConfig = function()
         {
         if (json)
            {
            jQuery("#" + wirelessNetworkingConfigurationMessageAreaId).empty();
            jQuery("#" + willStartWirelessNetworkingOnBootupCheckboxId).attr('checked', json["will-start-on-bootup"]);
            if (json['profiles'])
               {
               jQuery.each(json['profiles'], function(i, networkProfile)
                  {
                  addNetworkProfile(networkProfile);
                  });
               }

            notifyChangeListeners();
            }
         else
            {
            jQuery("#" + wirelessNetworkingConfigurationMessageAreaId).html("Sorry, the wireless configuration is currently unavailable.");
            }
         };

      this.addSelectionListener = function(listener)
         {
         if (listener)
            {
            selectionListeners[selectionListeners.length] = listener;
            }
         };

      this.addDoubleClickListener = function(listener)
         {
         if (listener)
            {
            doubleClickListeners[doubleClickListeners.length] = listener;
            }
         };

      this.addChangeListener = function(listener)
         {
         if (listener)
            {
            changeListeners[changeListeners.length] = listener;
            }
         };

      var getIsItemSelected = function()
         {
         return selectedWirelessNetwork != null;
         };
      this.isItemSelected = function()
         {
         return getIsItemSelected();
         };

      this.deleteSelected = function()
         {
         if (getIsItemSelected())
            {
            // delete the item from the list and fetch its uuid
            var uuid = jQuery(selectedWirelessNetwork).remove().attr("id");

            // delete it from the profiles map
            delete profilesMap[uuid];

            clearListSelection();

            // rebuild the JSON
            rebuildProfiles();
            }
         };

      this.addNetwork = function(networkProfile)
         {
         if (networkProfile)
            {
            addNetworkProfile(networkProfile);

            // rebuild the JSON
            rebuildProfiles();
            }
         };

      this.editNetwork = function(uuid, networkProfile)
         {
         if (uuid && networkProfile)
            {
            // save the new details in the profiles map
            profilesMap[uuid] = networkProfile;

            // update the UI
            jQuery("#" + uuid + "_info").text(networkProfile['ssid']);

            // rebuild the JSON
            rebuildProfiles();
            }
         };

      this.getSelectedItemJSON = function()
         {
         if (getIsItemSelected())
            {
            // get the selected item and fetch its uuid
            var uuid = jQuery(selectedWirelessNetwork).attr("id");

            return profilesMap[uuid];
            }
         };

      this.getSelectedItemUUID = function()
         {
         if (getIsItemSelected())
            {
            // get the selected item and return its uuid
            return jQuery(selectedWirelessNetwork).attr("id");
            }
         };

      this.setWillStartOnBootup = function(willStartOnBootup)
         {
         json["will-start-on-bootup"] = willStartOnBootup;
         notifyChangeListeners();
         };

      this.getJSON = function()
         {
         return JSON.stringify(json);
         };
      };

   // ==================================================================================================================
   })();

