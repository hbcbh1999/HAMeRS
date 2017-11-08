#include "apps/Navier-Stokes/NavierStokesInitialConditions.hpp"

/*
 * Set the data on the patch interior to some initial values.
 */
void
NavierStokesInitialConditions::initializeDataOnPatch(
    hier::Patch& patch,
    const std::vector<boost::shared_ptr<pdat::CellVariable<double> > >& conservative_variables,
    const boost::shared_ptr<hier::VariableContext>& data_context,
    const double data_time,
    const bool initial_time)
{
    NULL_USE(data_time);
    
    if ((d_project_name != "2D binary mass diffusion in x-direction") &&
        (d_project_name != "2D binary mass diffusion in y-direction"))
    {
        TBOX_ERROR(d_object_name
            << ": "
            << "Can only initialize data for 'project_name' = "
            << "'2D binary mass diffusion in x-direction' or "
            << "'2D binary mass diffusion in y-direction'"
            << "!\n"
            << "'project_name' = '"
            << d_project_name
            << "' is given."
            << std::endl);
    }
    
    if (d_dim != tbox::Dimension(2))
    {
        TBOX_ERROR(d_object_name
            << ": "
            << "Dimension of problem should be 2!"
            << std::endl);
    }
    
    if (d_flow_model_type != FLOW_MODEL::FOUR_EQN_CONSERVATIVE)
    {
        TBOX_ERROR(d_object_name
            << ": "
            << "Flow model should be conservative four-equation model!"
            << std::endl);
    }
    
    if (d_num_species != 2)
    {
        TBOX_ERROR(d_object_name
            << ": "
            << "Number of species should be 2!"
            << std::endl);
    }
    
    if (initial_time)
    {
        const boost::shared_ptr<geom::CartesianPatchGeometry> patch_geom(
            BOOST_CAST<geom::CartesianPatchGeometry, hier::PatchGeometry>(
                patch.getPatchGeometry()));
        
#ifdef HAMERS_DEBUG_CHECK_ASSERTIONS
        TBOX_ASSERT(patch_geom);
#endif
        
        const double* const dx = patch_geom->getDx();
        const double* const patch_xlo = patch_geom->getXLower();
        
        // Get the dimensions of box that covers the interior of Patch.
        hier::Box patch_box = patch.getBox();
        const hier::IntVector patch_dims = patch_box.numberCells();
        
        /*
         * Initialize data for 2D binary mass diffusion problem.
         */
        
        boost::shared_ptr<pdat::CellVariable<double> > var_partial_density = conservative_variables[0];
        boost::shared_ptr<pdat::CellVariable<double> > var_momentum        = conservative_variables[1];
        boost::shared_ptr<pdat::CellVariable<double> > var_total_energy    = conservative_variables[2];
        
        boost::shared_ptr<pdat::CellData<double> > partial_density(
            BOOST_CAST<pdat::CellData<double>, hier::PatchData>(
                patch.getPatchData(var_partial_density, data_context)));
        
        boost::shared_ptr<pdat::CellData<double> > momentum(
            BOOST_CAST<pdat::CellData<double>, hier::PatchData>(
                patch.getPatchData(var_momentum, data_context)));
        
        boost::shared_ptr<pdat::CellData<double> > total_energy(
            BOOST_CAST<pdat::CellData<double>, hier::PatchData>(
                patch.getPatchData(var_total_energy, data_context)));
        
#ifdef HAMERS_DEBUG_CHECK_ASSERTIONS
        TBOX_ASSERT(partial_density);
        TBOX_ASSERT(momentum);
        TBOX_ASSERT(total_energy);
#endif
        
        double* rho_Y_1 = partial_density->getPointer(0);
        double* rho_Y_2 = partial_density->getPointer(1);
        double* rho_u   = momentum->getPointer(0);
        double* rho_v   = momentum->getPointer(1);
        double* E       = total_energy->getPointer(0);
        
        if (d_project_name == "2D binary mass diffusion in x-direction")
        {
            const double gamma = double(7)/double(5);
            
            // Characteristic length of the problem.
            const double K = double(1)/double(100);
            
            // Initial conditions of mixture.
            const double rho = double(1);
            const double u   = double(0);
            const double v   = double(0);
            const double p   = double(1);
            
            for (int j = 0; j < patch_dims[1]; j++)
            {
                for (int i = 0; i < patch_dims[0]; i++)
                {
                    // Compute index into linear data array.
                    int idx_cell = i + j*patch_dims[0];
                    
                    // Compute the coordinates.
                    double x[2];
                    x[0] = patch_xlo[0] + (double(i) + double(1)/double(2))*dx[0];
                    x[1] = patch_xlo[1] + (double(j) + double(1)/double(2))*dx[1];
                    
                    const double Y_1 = exp(-x[0]*x[0]/K);
                    const double Y_2 = 1.0 - Y_1;
                    
                    rho_Y_1[idx_cell] = rho*Y_1;
                    rho_Y_2[idx_cell] = rho*Y_2;
                    rho_u[idx_cell]   = rho*u;
                    rho_v[idx_cell]   = rho*v;
                    E[idx_cell]       = p/(gamma - double(1)) + double(1)/double(2)*rho*(u*u + v*v);
                }
            }
        }
        else if (d_project_name == "2D binary mass diffusion in y-direction")
        {
            const double gamma = double(7)/double(5);
            
            // Characteristic length of the problem.
            const double K = double(1)/double(100);
            
            // Initial conditions of mixture.
            const double rho = double(1);
            const double u   = double(0);
            const double v   = double(0);
            const double p   = double(1);
            
            for (int j = 0; j < patch_dims[1]; j++)
            {
                for (int i = 0; i < patch_dims[0]; i++)
                {
                    // Compute index into linear data array.
                    int idx_cell = i + j*patch_dims[0];
                    
                    // Compute the coordinates.
                    double x[2];
                    x[0] = patch_xlo[0] + (double(i) + double(1)/double(2))*dx[0];
                    x[1] = patch_xlo[1] + (double(j) + double(1)/double(2))*dx[1];
                    
                    const double Y_1 = exp(-x[1]*x[1]/K);
                    const double Y_2 = 1.0 - Y_1;
                    
                    rho_Y_1[idx_cell] = rho*Y_1;
                    rho_Y_2[idx_cell] = rho*Y_2;
                    rho_u[idx_cell]   = rho*u;
                    rho_v[idx_cell]   = rho*v;
                    E[idx_cell]       = p/(gamma - double(1)) + double(1)/double(2)*rho*(u*u + v*v);
                }
            }
        }
    }
}