#include "subsys/logging.hpp"

namespace logging
{
    bool file_exists(const std::string& name)
    {
        ifstream f(name.c_str());
        return f.good();
    }

    string get_filename()
    {
        int possibleMax = 10000;

        for (int i=0; i<possibleMax; i++)
        {
            string fname = "";
            fname.append(path);
            fname.append(prefix);
            fname.append(to_string(i));
            fname.append(extension);
            if (!file_exists(fname))
                return fname;
        }
        return path + prefix + "0" + extension;
    }

    void init()
    {
        log_file.open(get_filename());
    }
}