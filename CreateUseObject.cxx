#include <stdio.h>
#include <sal/main.h>
#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/frame/XDesktop2.hpp>

using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::frame;

using ::rtl::OString;
using ::rtl::OUString;
using ::rtl::OUStringToOString;

SAL_IMPLEMENT_MAIN()
{
    try
    {
        // get the remote office component context
        Reference< XComponentContext > xContext( ::cppu::bootstrap() );
	if ( !xContext.is() )
	{
	    fprintf(stdout, "\nError getting context from running LO instance...\n");
	    return -1;
	}
	
	// retrieve the service-manager from the context
        Reference< XMultiComponentFactory > xServiceManager = xContext->getServiceManager();
	if ( xServiceManager.is() )
	    fprintf(stdout, "\nremote ServiceManager is available\n");
	else
	    fprintf(stdout, "\nremote ServiceManager is not available\n");
	fflush(stdout);

	// Create Desktop object, xDesktop is still a XInterface, need to upcast it via queryInterface.
	Reference< XInterface > xDesktop = xServiceManager->createInstanceWithContext( OUString("com.sun.star.frame.Desktop"), xContext );

	if ( !xDesktop.is() )
	{
	    fprintf(stdout, "\nError creating com.sun.star.frame.Desktop object");
	    fflush(stdout);
	    return -1;
	}

	// Get XDesktop2 from XInterface type (upcast)
	Reference< XDesktop2 > xDesktop2( xDesktop, UNO_QUERY );

	if ( !xDesktop2.is() )
	{
	    fprintf(stdout, "\nError upcasting XInterface to XDesktop2");
	    fflush(stdout);
	    return -1;
	}

	// open a spreadsheet document                                                                                                                                                                 
        Reference< XComponent > xComponent = xDesktop2->loadComponentFromURL( OUString( "private:factory/scalc" ), // URL to the ods file
									      OUString( "_blank" ), 0,
									      Sequence < ::com::sun::star::beans::PropertyValue >() );
	if ( !xComponent.is() )
	{
	    fprintf(stdout, "\nopening spreadsheet document failed!\n");
	    fflush(stdout);
	    return -1;
	}
	
	fprintf(stdout, "\nopened spreadsheet document...\n");
	fflush(stdout);
	
    }
    catch ( ::cppu::BootstrapException& e )
    {
        fprintf(stderr, "caught BootstrapException: %s\n",
                OUStringToOString( e.getMessage(), RTL_TEXTENCODING_ASCII_US ).getStr());
	fflush(stderr);
        return 1;
    }
    catch ( Exception& e )
    {
        fprintf(stderr, "caught UNO exception: %s\n",
                OUStringToOString( e.Message, RTL_TEXTENCODING_ASCII_US ).getStr());
	fflush(stderr);
        return 1;
    }

    return 0;

}
