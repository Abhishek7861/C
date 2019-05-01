#include <bits/stdc++.h>
using namespace std;

struct udp_header
{
  uint16_t source_port;
  uint16_t dest_port;
  uint16_t length;
  uint16_t checksum;

  string encode()
  {
    return to_string(source_port) + " " + to_string(dest_port) + " " + to_string(length) + " " + to_string(checksum);
  }

  void decode(string udpHeader, struct udp_header *udphdr)
  {
    vector<string> tokens;
    stringstream check1(udpHeader);
    string intermediate;
    // Tokenizing w.r.t. space ' '
    while (getline(check1, intermediate, ' '))
    {
      tokens.push_back(intermediate);
    }

    udphdr->source_port = stoi(tokens[0]);
    udphdr->dest_port = stoi(tokens[1]);
    udphdr->length = stoi(tokens[2]);
    udphdr->checksum = stoi(tokens[3]);
  }
};

struct ip_header
{
  uint8_t version : 4;
  uint8_t length : 4;
  uint8_t typeOfService;
  uint16_t totallength;
  uint16_t identification;
  uint8_t odfm : 3;
  uint16_t fragment_Offset;
  uint8_t timetolive;
  uint8_t protocol;
  uint16_t checksum;
  uint32_t sourceAddress;
  uint32_t destinationAddress;

  string encode()
  {
    return to_string(version) + " " + to_string(length) + " " + to_string(typeOfService) + " " + to_string(totallength) + " " +
           to_string(identification) + " " + to_string(odfm) + " " + to_string(fragment_Offset) + " " + to_string(timetolive) + " " +
           to_string(protocol) + " " + to_string(checksum) + " " + to_string(sourceAddress) + " " + to_string(destinationAddress);
  }

  void decode(string ipHeader, struct ip_header *iphdr)
  {
    vector<string> tokens;
    stringstream check1(ipHeader);
    string intermediate;
    // Tokenizing w.r.t. space ' '
    while (getline(check1, intermediate, ' '))
    {
      tokens.push_back(intermediate);
    }
    iphdr->version = stoi(tokens[0]);
    iphdr->length = stoi(tokens[1]);
    iphdr->typeOfService = stoi(tokens[2]);
    iphdr->totallength = stoi(tokens[3]);
    iphdr->identification = stoi(tokens[4]);
    iphdr->odfm = stoi(tokens[5]);
    iphdr->fragment_Offset = stoi(tokens[6]);
    iphdr->timetolive = stoi(tokens[7]);
    iphdr->protocol = stoi(tokens[8]);
    iphdr->checksum = stoi(tokens[9]);
    iphdr->sourceAddress = stoi(tokens[10]);
    iphdr->destinationAddress = stoi(tokens[11]);
  }
};

struct wireBuffer
{
  string udp;
  string ip;
  string msg;

  string encode()
  {
    return msg + " "+ udp + " " + ip;
  }
  void decode(string Msg, struct wireBuffer *msgStruct)
  {
    vector<string> tokens;
    stringstream check1(Msg);
    string intermediate;
    // Tokenizing w.r.t. space ' '
    while (getline(check1, intermediate, ' '))
    {
      tokens.push_back(intermediate);
    }
    msgStruct->msg = tokens[0];
    msgStruct->udp = tokens[1];
    msgStruct->ip = tokens[2];

  }

};

int main()
{
  string msg = "hello";
  struct udp_header udphdr
  {
    1, 2, 3, 4
  };

  struct ip_header iphdr{
    1,2,3,4,5,6,7,8,9,10,11,12
  };
  struct wireBuffer wiremsg{
    udphdr.encode(),iphdr.encode(),msg
  };
  cout<< wiremsg.encode()<<endl;
}
