#include <regex>
#include <iterator>
#include <iostream>
#include <string>
#include <string_view>
#include <boost/regex.hpp>
#include <vector>


// link with lboost_regex
 
int main()
{
#if 0
    const std::string s = "23423452345\n2345234562\nSMART support is: Enabled";
 
    std::regex words_regex("SMART support is: (\\w+)");
    auto words_begin = 
        std::sregex_token_iterator(s.begin(), s.end(), words_regex, 1);
    auto words_end = std::sregex_token_iterator();
 
    std::cout << "Found " 
              << std::distance(words_begin, words_end) 
              << " words:\n";
    std::string_view match;
    std::cout<< (match == "Enabled") << std::endl;
 while(words_begin != words_end) {
    match = std::string_view{&(*words_begin->first), static_cast<size_t>(std::distance(words_begin->first, words_begin->second))};
    std::cout<<match<<std::endl;
    std::cout<<(match  == "Enabled")<<std::endl;
    words_begin++;
 }   
#endif 
 const char * long_text = R"(smartctl 6.5 2016-05-07 r4318 [x86_64-linux-4.15.17] (local build)
Copyright (C) 2002-16, Bruce Allen, Christian Franke, www.smartmontools.org

=== START OF READ SMART DATA SECTION ===
SMART Attributes Data Structure revision number: 1
Vendor Specific SMART Attributes with Thresholds:
ID# ATTRIBUTE_NAME          FLAG     VALUE WORST THRESH TYPE      UPDATED  WHEN_FAILED RAW_VALUE
  5 Reallocated_Sector_Ct   0x0032   100   100   000    Old_age   Always       -       5
  9 Power_On_Hours          0x0032   100   100   000    Old_age   Always       -       15677
 12 Power_Cycle_Count       0x0032   100   100   000    Old_age   Always       -       60
175 Program_Fail_Count_Chip 0x0033   100   100   010    Pre-fail  Always       -       627506809323
183 Runtime_Bad_Block       0x0032   100   100   000    Old_age   Always       -       0
184 End-to-End_Error        0x0033   100   100   090    Pre-fail  Always       -       0
187 Reported_Uncorrect      0x0032   100   100   000    Old_age   Always       -       0
190 Airflow_Temperature_Cel 0x0022   078   078   000    Old_age   Always       -       22 (Min/Max 21/25)
192 Power-Off_Retract_Count 0x0032   100   100   000    Old_age   Always       -       43
194 Temperature_Celsius     0x0022   100   100   000    Old_age   Always       -       22
197 Current_Pending_Sector  0x0012   100   100   000    Old_age   Always       -       0
199 UDMA_CRC_Error_Count    0x003e   100   100   000    Old_age   Always       -       0
228 Power-off_Retract_Count 0x0032   100   100   000    Old_age   Always       -       940264
232 Available_Reservd_Space 0x0033   099   099   010    Pre-fail  Always       -       0
233 Media_Wearout_Indicator 0x0032   100   100   000    Old_age   Always       -       0
234 Unknown_Attribute       0x0032   100   100   000    Old_age   Always       -       0
241 Total_LBAs_Written      0x0032   100   100   000    Old_age   Always       -       15689
242 Total_LBAs_Read         0x0032   100   100   000    Old_age   Always       -       43)";

  std::string text{long_text};
  // std::cout<<text<<std::endl;
  //boost::regex reg("^(\\d+)\\s+.*\\s+(\\d+.*$)");
  //[ -~] matching any printable ASCII repeating 84 times
  boost::regex reg("^\\s*(\\d+)[ -~]{84}(\\d+)");
  int const subs[] = { 1, 2 };
  boost::sregex_token_iterator it{text.begin(), text.end(),reg, subs};
  boost::sregex_token_iterator end;
  std::vector<std::string> results;
  while(it != end) {
    std::cout<<*it<<"\t";
    results.emplace_back(*it);
    it++;
    std::cout<<*it<<std::endl;
    results.emplace_back(*it);
    it++;
 }   

  std::cout<< results.size() << std::endl;

  const char *scan = R"(
/dev/sda -d scsi # /dev/sda, SCSI device
/dev/sdb -d scsi # /dev/sdb, SCSI device
/dev/sdc -d scsi # /dev/sdc, SCSI device
/dev/sdd -d scsi # /dev/sdd, SCSI device
/dev/sde -d scsi # /dev/sde, SCSI device
/dev/sdf -d scsi # /dev/sdf, SCSI device
)";
  std::string scan_text(scan);
  boost::regex scan_re("^(/dev/sd[a-z])");
  boost::sregex_token_iterator it1{scan_text.begin(), scan_text.end(), scan_re, 1};
  boost::sregex_token_iterator end1;
  while(it1 != end1) {
    std::cout<<*it1<<"\t";
    results.emplace_back(*it1);
    it1++;
 }   

  std::cout<< results.size() << std::endl;
}
