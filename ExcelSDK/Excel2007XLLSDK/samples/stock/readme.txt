This is a sample XLL and XLAM demonstrating use of the Framework as well
as the variety of possible projects enabled by the SDK.  The XLL contains
all of the UDF's used in the add-in, and can also be used as a standalone
if you want access to the functions it defines.  

Loading the XLAM will give an extra item on the ribbon to control generating
a workbook and the accompanying options.  The code is viewable in the XLAM
by going to the Developer tab and clicking the Visual Basic editor.

The Web Service is currently set to poll http://localhost:1618/ and assumes
the service lives at http://localhost:1618/Service.asmx

To change those assumptions, you can either edit the files in the StockService
sub directory, or open the project in Visual Studio and add a web reference
pointing at the desired address.  The name of the web service MUST be
StockService.h unless the include files and makefile are changed.

It is fine to not have the web service running. Default behavior is remote
polling of data from the web service, but can be changed by the XLAM or a 
Visual Basic call (as seen in the XLAM).  Disabling the web service will poll 
from local data; the numbers are in StockServiceProxy.cpp.
