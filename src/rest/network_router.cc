
#include <vector>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "utility/utility.h"
#include "system/system_manager.h"
#include "rest/api_endpoint.h"
#include "rest/network_router.h"

NetworkRouter::NetworkRouter() {
  network_manager_ = NetworkManager::GetInstance();
}

void NetworkRouter::PostWirelessConnection(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }
  
  json j;
  std::string body;

  body = request.body();
  j = json::parse(body);

  std::string ssid = j["ssid"];
  std::string psk = j["psk"];
  std::string security = j["security"];

  network_manager_->AddNetwork(ssid, psk, security);
  response.send(Pistache::Http::Code::Ok, "");
}

void NetworkRouter::GetWirelessConnection(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }

  json j;
  WpaStatus wpa_status = network_manager_->GetWpaStatus(); 
  j["wpa_state"]= wpa_status.wpa_state;
  j["bssid"] = wpa_status.bssid;
  j["ssid"] = wpa_status.ssid;
  j["freq"] = wpa_status.freq;
  j["key_mgmt"] = wpa_status.key_mgmt;
  PLOGI("%s", j.dump().c_str());
  response.send(Pistache::Http::Code::Ok, j.dump());
}

void NetworkRouter::GetNetworkStatus(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }

  json j;
  j["stationIpAddr"] = network_manager_->GetStationStatus();
  j["softapIpAddr"] = network_manager_->GetSoftapStatus();
  j["usbIpAddr"] = network_manager_->GetUsbEthernetStatus();
  j["netmask"] = network_manager_->GetStationNetmask();
  j["hwAddr"] = network_manager_->GetStationHwAddr();
  PLOGI("%s", j.dump().c_str());
  response.send(Pistache::Http::Code::Ok, j.dump());
}

void NetworkRouter::GetHotspot(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }
   
  std::vector<WpaInfo> wpa_infos = network_manager_->Scan();

  json res = json::array();
  json j;
  WpaInfo wpa_info;
  int sig;
  std::string strength;
  std::string security;

  for(size_t i = 0; i < wpa_infos.size(); ++i) {
    WpaInfo wpa_info = wpa_infos[i];
    sig = wpa_info.signal;
    if(sig > -50)
      strength = "4";
    else if(sig > -70)
      strength = "3";
    else if(sig > -80)
      strength = "2";
    else
      strength = "1";   

    if(wpa_info.flags.find("PSK") != std::string::npos) {
      security = "WPA-PSK";
    }
    else {
      security = "None";
    }

    j = {{"ssid", wpa_info.ssid}, {"strength", strength}, {"security", security}, {"status", "None"}};
    res.push_back(j);
  }
  PLOGI("%s", res.dump().c_str());
  response.send(Pistache::Http::Code::Ok, res.dump());
}


