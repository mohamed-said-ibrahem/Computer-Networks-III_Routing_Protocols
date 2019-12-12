#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("P2P_Topology_Simualtion");

int main (int argc, char *argv[]){
    Time::SetResolution (Time::NS);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);

    NS_LOG_LOGIC ("Creating Network Nodes...");
    // Creating Network Nodes
    NodeContainer p2pNodes;
    p2pNodes.Create(6);

    NS_LOG_LOGIC ("Installing Internet stacks...");
    //Install Internet stacks on each node
    InternetStackHelper stack;
    stack.Install(p2pNodes);
    // Create point-to-point helper PointToPointHelper p2p;
    PointToPointHelper pointToPoint;
    // Create an Address helper
    Ipv4AddressHelper Address;

    // Create subnets
    NS_LOG_LOGIC ("Creating subnets...");
    NodeContainer n01 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get (1));
    NodeContainer n02 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get (2));
    NodeContainer n12 = NodeContainer (p2pNodes.Get (1), p2pNodes.Get (2));
    NodeContainer n13 = NodeContainer (p2pNodes.Get (1), p2pNodes.Get (3));
    NodeContainer n14 = NodeContainer (p2pNodes.Get (1), p2pNodes.Get (4));
    NodeContainer n24 = NodeContainer (p2pNodes.Get (2), p2pNodes.Get (4));
    NodeContainer n34 = NodeContainer (p2pNodes.Get (3), p2pNodes.Get (4));
    NodeContainer n45 = NodeContainer (p2pNodes.Get (4), p2pNodes.Get (5));

    // Assigning IPs
    NS_LOG_LOGIC ("Assigning IPs...");
    Address.SetBase ("10.1.1.0", "255.255.255.0");
    NetDeviceContainer nd01 = pointToPoint.Install(n01);
    Ipv4InterfaceContainer i01 = Address.Assign(nd01);

    Address.SetBase ("10.1.2.0", "255.255.255.0");
    NetDeviceContainer nd02 = pointToPoint.Install(n02);
    Ipv4InterfaceContainer i02 = Address.Assign(nd02);

    Address.SetBase ("10.1.3.0", "255.255.255.0");
    NetDeviceContainer nd12 = pointToPoint.Install(n12);
    Ipv4InterfaceContainer i12 = Address.Assign(nd12);

    Address.SetBase ("10.1.4.0", "255.255.255.0");
    NetDeviceContainer nd13 = pointToPoint.Install(n13);
    Ipv4InterfaceContainer i13 = Address.Assign(nd13);

    Address.SetBase ("10.1.5.0", "255.255.255.0");
    NetDeviceContainer nd14 = pointToPoint.Install(n14);
    Ipv4InterfaceContainer i14 = Address.Assign(nd14);

    Address.SetBase ("10.1.6.0", "255.255.255.0");
    NetDeviceContainer nd24 = pointToPoint.Install(n24);
    Ipv4InterfaceContainer i24 = Address.Assign(nd24);

    Address.SetBase ("10.1.7.0", "255.255.255.0");
    NetDeviceContainer nd34 = pointToPoint.Install(n34);
    Ipv4InterfaceContainer i34 = Address.Assign(nd34);

    Address.SetBase ("10.1.8.0", "255.255.255.0");
    NetDeviceContainer nd45 = pointToPoint.Install(n45);
    Ipv4InterfaceContainer i45 = Address.Assign(nd45);

    NS_LOG_LOGIC ("Create Server & Client(s)..");
    // server
    uint16_t port = 9;
    UdpEchoServerHelper server(port);
    ApplicationContainer serverApp = server.Install(p2pNodes.Get(5));
    serverApp.Start(Seconds (1.0));
    serverApp.Stop(Seconds (10.0));
    // client
    UdpEchoClientHelper client(i45.GetAddress(1), port);
    client.SetAttribute("MaxPackets", UintegerValue(1));
    client.SetAttribute("Interval", TimeValue(Seconds (1.0)));
    client.SetAttribute("PacketSize", UintegerValue(1024));
    ApplicationContainer clientApp = client.Install(p2pNodes.Get(0));
    clientApp.Start(Seconds (2.0));
    clientApp.Stop(Seconds (10.0));

    NS_LOG_LOGIC ("Create & print rounting table");
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    // pointToPoint.EnablePcapAll("second");
    Ipv4GlobalRoutingHelper g;
    Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>
    ("global-routing.routes", std::ios::out);
    g.PrintRoutingTableAllAt (Seconds (10), routingStream);

    NS_LOG_LOGIC ("Create Animation interface..");
    AnimationInterface anim ("animation.xml");
    anim.SetConstantPosition(p2pNodes.Get(0), 10.0, 50.0);
    anim.SetConstantPosition(p2pNodes.Get(1), 30.0, 30.0);
    anim.SetConstantPosition(p2pNodes.Get(2), 30.0, 70.0);
    anim.SetConstantPosition(p2pNodes.Get(3), 70.0, 30.0);
    anim.SetConstantPosition(p2pNodes.Get(4), 70.0, 70.0);
    anim.SetConstantPosition(p2pNodes.Get(5), 90.0, 70.0);

    Simulator::Stop (Seconds (30));
    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}