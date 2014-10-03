using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Web.Services.Protocols;

/// Summary description for StockInfoHeader
public class StockInfoHeader : SoapHeader
{
    public double open;
    public double close;
    public double low;
    public double high;

    public StockInfoHeader()
        : this(-1, -1, -1, -1)
	{
		//
		// TODO: Add constructor logic here
		//
	}

    public StockInfoHeader(double open, double close, double low, double high)
    {
        this.open = Math.Round(open, 2);
        this.close = Math.Round(close, 2);
        this.low = Math.Round(low, 2);
        this.high = Math.Round(high, 2);
    }
}
