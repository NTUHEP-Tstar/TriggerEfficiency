#ifndef PATHMGR_HPP
#define PATHMGR_HPP

#include <map>
#include <string>

namespace dra
{

class Pathmgr
{
public:
  Pathmgr(
    const std::string& package,
    const std::string& sub_package,
    const std::string& settings_dir = "settings",
    const std::string& results_dir = "results",
    const std::string& datas_dir = "data"
    );
  virtual
  ~Pathmgr ();

  // Instance functions for getting file names
  std::string PackageDir()    const;
  std::string SubPackageDir() const;

  // Default settings for sample
  std::string SettingsDir() const;
  std::string DatasDir() const;
  std::string ResultsDir()  const;
  std::string SubDir( const std::string& ) const;

private:
  const std::string _package_name;
  const std::string _sub_package_name;
  const std::string _settings_dir;
  const std::string _results_dir;
  const std::string _datas_dir;
};

}

#endif
