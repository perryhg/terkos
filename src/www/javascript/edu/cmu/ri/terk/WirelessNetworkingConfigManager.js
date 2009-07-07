//======================================================================================================================
// Class for managing the wireless networking configuration.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
// * Math.uuid (http://www.broofa.com/blog/?p=151)
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
   var msg = "The jQuery library is required by edu.cmu.ri.terk.WirelessNetworkingConfigManager.js";
   alert(msg);
   throw new Error(msg);
   }
//======================================================================================================================
if (!Math.uuid)
   {
   var msg = "The Math.uuid library is required by edu.cmu.ri.terk.WirelessNetworkingConfigManager.js";
   alert(msg);
   throw new Error(msg);
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
                                                              wirelessNetworksListId)
      {
      var host = '';//http://192.168.0.4'; // TODO: remove me!

      var selectedWirelessNetwork = null;
      var selectionListeners = new Array();
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
         var desiredProfileOrdering = $('#' + wirelessNetworksListId).sortable("toArray");

         // add the profiles to the JSON in the new order
         for (var i = 0; i < desiredProfileOrdering.length; i++)
            {
            var uuid = desiredProfileOrdering[i];
            json['profiles'][json['profiles'].length] = profilesMap[uuid];
            }

         displayJsonForDebugging();
         };

      var displayJsonForDebugging = function()
         {
         jQuery("#info").html("<pre>" + JSON.stringify(json, null, "\t") + "</pre>");
         };

      this.getWirelessNetworkingConfig = function()
         {
         jQuery("#" + wirelessNetworkingConfigurationMessageAreaId).html("Loading preferences...");
         jQuery("#" + wirelessNetworkingConfigurationAreaId).addClass("hidden");

         // clear the list
         jQuery("#" + wirelessNetworksListId).empty();

         clearListSelection();

         // load the wireless config
         jQuery.ajax(
         {
            url: host + '/cgi-bin/getWirelessNetworkingConfigAsJSON.pl',
            success: function(jsonResponse)
               {
               jQuery("#" + wirelessNetworkingConfigurationAreaId).removeClass("hidden");
               json = jsonResponse;
               displayWirelessNetworkingConfig();
               },
            error: function()
               {
               json = null;
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

            displayJsonForDebugging();
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

      this.isItemSelected = function()
         {
         return selectedWirelessNetwork != null;
         };

      this.deleteSelected = function()
         {
         if (this.isItemSelected())
            {
            // delete the item from the list and fetch its uuid
            var uuid = jQuery(selectedWirelessNetwork).remove().attr("id");

            // delete it from the profiles map
            delete profilesMap[uuid];

            clearListSelection();

            // rebuild the JSON
            rebuildProfiles();
            }
         }

      this.editSelected = function()
         {
         if (this.isItemSelected())
            {
            // TODO
            alert("Edit not yet implemented");
            }
         };

      this.addNetwork = function(networkProfile)
         {
         if (networkProfile)
            {
            addNetworkProfile(networkProfile)

            // rebuild the JSON
            rebuildProfiles();
            }
         };

      this.setWillStartOnBootup = function(willStartOnBootup)
         {
         json["will-start-on-bootup"] = willStartOnBootup;
         displayJsonForDebugging();
         };
      };
   // ==================================================================================================================


   // ==================================================================================================================
   })();

