//======================================================================================================================
// Class for managing the wireless networking configuration.
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
   var nojQueryMsg = "The jQuery library is required by edu.cmu.ri.terk.WirelessNetworkingConfigManager.js";
   alert(nojQueryMsg);
   throw new Error(nojQueryMsg);
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
      var host = '';//http://192.168.1.104'; // TODO: remove me!

      var selectedWirelessNetwork = null;
      var selectionListeners = new Array();

      jQuery("#" + wirelessNetworksListId).sortable({
         handle : '.handle',
         axis: 'y',
         tolerance: 'pointer' ,
         containment: '#' + wirelessNetworksListContainerId
      }).disableSelection();

      this.getWirelessNetworkingConfig = function()
         {
         jQuery("#" + wirelessNetworkingConfigurationMessageAreaId).html("Loading preferences...");
         jQuery("#" + wirelessNetworkingConfigurationAreaId).addClass("hidden");

         // clear the list
         jQuery("#" + wirelessNetworksListId).empty();

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

         // load the wireless config
         jQuery.ajax(
         {
            url: host + '/cgi-bin/getWirelessNetworkingConfigAsJSON.pl',
            success: function(jsonResponse)
               {
               jQuery("#" + wirelessNetworkingConfigurationAreaId).removeClass("hidden");
               displayWirelessNetworkingConfig(jsonResponse);
               },
            error: function()
               {
               displayWirelessNetworkingConfig(null);
               }
         });
         };

      var displayWirelessNetworkingConfig = function(wirelessNetworkingConfig)
         {
         if (wirelessNetworkingConfig)
            {
            jQuery("#" + wirelessNetworkingConfigurationMessageAreaId).empty();
            jQuery("#" + willStartWirelessNetworkingOnBootupCheckboxId).attr('checked', wirelessNetworkingConfig["will-start-on-bootup"]);
            if (wirelessNetworkingConfig['profiles'])
               {
               jQuery.each(wirelessNetworkingConfig['profiles'], function(i, networkProfile)
                  {
                  if (networkProfile)
                     {
                     // build the list item
                     var listItem = document.createElement("div");
                     listItem.id = "preferredWirelessNetworksListItem_" + i;
                     listItem.className = "sortable-list-item";

                     var listItemInfo = document.createElement("div");
                     listItemInfo.id = "preferredWirelessNetworksListItemInfo_" + i;
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
                                 if (selectedWirelessNetwork != null)
                                    {
                                    jQuery(selectedWirelessNetwork).removeClass("selected");
                                    }
                                 jQuery(this).addClass("selected");
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
                  });
               }
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
      };
   // ==================================================================================================================


   // ==================================================================================================================
   })();

