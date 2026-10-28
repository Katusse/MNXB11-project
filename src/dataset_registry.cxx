#include "dataset_registry.h"

#include <map>
#include <stdexcept>

std::string get_dataset_path(const std::string& name)
{
    static const std::map<std::string, std::string> datasets = {
        {
            "lund",
            "datasets/clean_smhi-opendata_1_53430_20261007_164857_Lund.csv"
        },
        {
            "umea",
            "datasets/clean_smhi-opendata_1_140480_20261007_164814_Umeå_Flygplats.csv"
        },
        {
            "lulea",
            "datasets/clean_smhi-opendata_1_162860_20261007_164918_Luleå-Kallax_Flygplats.csv"
        },
        {
            "falsterbo",
            "datasets/clean_smhi-opendata_1_52230_20261007_165001_Falsterbo.csv"
        },
        {
            "halmstad",
            "datasets/clean_smhi-opendata_1_62410_20261007_164836_Halmstad_flygplats.csv"
        },
        {
            "boras",
            "datasets/clean_smhi-opendata_1_72450_20261007_164940_Borås.csv"
        },
        {
            "visby",
            "datasets/clean_smhi-opendata_1_78400_20261007_165022_Visby_Flygplats.csv"
        },
        {
            "karlstad",
            "datasets/clean_smhi-opendata_1_93220_20261007_164752_Karlstad_Flygplats.csv"
        },
        {
            "uppsala",
            "datasets/clean_smhi-opendata_1_97530_20261007_165105_Uppsala_Flygplats.csv"
        },
        {
            "soderarm",
            "datasets/clean_smhi-opendata_1_99450_20261007_164731_Söderarm_A.csv"
        },
    };

    auto it = datasets.find(name);

    if (it == datasets.end()) {
        throw std::runtime_error(
            "Unknown dataset: " + name
        );
    }

    return it->second;
}