//======================================================================================================================
// Class for scanning for wireless networks.
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
   var nojQueryMsg = "The jQuery library is required by edu.cmu.ri.terk.WirelessNetworkScanner.js";
   alert(nojQueryMsg);
   throw new Error(nojQueryMsg);
   }
//======================================================================================================================
if (!Math.uuid)
   {
   var msg2 = "The Math.uuid library is required by edu.cmu.ri.terk.WirelessNetworkingConfigManager.js";
   alert(msg2);
   throw new Error(msg2);
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

   edu.cmu.ri.terk.WirelessNetworkScanner = function(wirelessNetworksListId,
                                                     pleaseWaitIndicatorId)
      {
      var host = '';//http://192.168.1.4'; // TODO: remove me!

      var selectedWirelessNetwork = null;
      var selectionListeners = new Array();
      var profilesMap = new Array();

      this.scanForWirelessNetworks = function()
         {
         jQuery("#" + pleaseWaitIndicatorId).removeClass("hidden");

         // clear the list
         jQuery("#" + wirelessNetworksListId).empty();

         // reset the selected network
         selectedWirelessNetwork = null;

         // notify listeners that nothing is selected
         jQuery.each(selectionListeners, function(i, listener)
            {
            if (listener)
               {
               listener(false);
               }
            });

         // load the network list
         jQuery.ajax(
         {
            url: host + '/cgi-bin/getWirelessNetworksAsJSON.pl',
            data: "include-encrypted=false",
            success: function(wirelessNetworksJSON)
               {
               displayAvailableWirelessNetworks(wirelessNetworksJSON);
               },
            error: function()
               {
               displayAvailableWirelessNetworks(null);
               }
         });
         };

      var createUUID = function()
         {
         return Math.uuid(10);
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

            jQuery("#" + wirelessNetworksListId).append(listItem);
            jQuery("#" + listItem.id).text(networkProfile['ssid']).disableSelection().addClass("list-item");

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
         };

      var displayAvailableWirelessNetworks = function(wirelessNetworksJSON)
         {
         jQuery("#" + pleaseWaitIndicatorId).addClass("hidden");

         if (wirelessNetworksJSON && wirelessNetworksJSON['wireless-networks'])
            {
            var wirelessNetworks = wirelessNetworksJSON['wireless-networks'];
            if (wirelessNetworks && wirelessNetworks.length > 0)
               {
               jQuery.each(wirelessNetworks, function(i, networkProfile)
                  {
                  addNetworkProfile(networkProfile);
                  });
               }
            }
         else
            {
            // TODO
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

      this.getSelectedItemJSON = function()
         {
         if (this.isItemSelected())
            {
            // get the selected item and fetch its uuid
            var uuid = jQuery(selectedWirelessNetwork).attr("id");
            return profilesMap[uuid];
            }
         };

      };
   // ==================================================================================================================
   })();

