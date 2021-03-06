#ifndef EQUATION_OF_THERMAL_CONDUCTIVITY_MIXING_RULES_MANAGER_HPP
#define EQUATION_OF_THERMAL_CONDUCTIVITY_MIXING_RULES_MANAGER_HPP

#include "HAMeRS_config.hpp"

#include "util/mixing_rules/equations_of_thermal_conductivity/EquationsOfThermalConductivity.hpp"

#include "SAMRAI/tbox/Dimension.h"

#include "boost/shared_ptr.hpp"
#include <string>

using namespace SAMRAI;

class EquationOfThermalConductivityMixingRulesManager
{
    public:
        EquationOfThermalConductivityMixingRulesManager(
            const std::string& object_name,
            const tbox::Dimension& dim,
            const int& num_species,
            const MIXING_CLOSURE_MODEL::TYPE& mixing_closure_model,
            const boost::shared_ptr<tbox::Database>& equation_of_thermal_conductivity_mixing_rules_db,
            const std::string& equation_of_thermal_conductivity_str);
        
        /*
         * Get the type of equation of thermal conductivity.
         */
        const EQN_THERMAL_CONDUCTIVITY::TYPE&
        getEquationOfThermalConductivityType() const
        {
            return d_equation_of_thermal_conductivity_type;
        }
        
        /*
         * Get the equation of thermal conductivity mixing rules.
         */
        boost::shared_ptr<EquationOfThermalConductivityMixingRules>
        getEquationOfThermalConductivityMixingRules() const
        {
            return d_equation_of_thermal_conductivity_mixing_rules;
        }
        
        /*
         * Print all characteristics of equation of thermal conductivity manager.
         */
        void
        printClassData(std::ostream& os) const;
        
    private:
        /*
         * The object name is used for error/warning reporting.
         */
        const std::string d_object_name;
        
        /*
         * Type of equation of thermal conductivity.
         */
        EQN_THERMAL_CONDUCTIVITY::TYPE d_equation_of_thermal_conductivity_type;
        
        /*
         * boost::shared_ptr to the equation of thermal conductivity.
         */
        boost::shared_ptr<EquationOfThermalConductivityMixingRules> d_equation_of_thermal_conductivity_mixing_rules;
        
};

#endif /* EQUATION_OF_THERMAL_CONDUCTIVITY_MIXING_RULES_MANAGER_HPP */
