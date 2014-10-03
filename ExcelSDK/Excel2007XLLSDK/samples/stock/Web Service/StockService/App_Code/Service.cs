using System;
using System.Web;
using System.Web.Services;
using System.Web.Services.Protocols;

using System.Collections.Generic;
using System.Threading;

namespace Testing
{

    [WebService(Namespace = "http://imaginarystockservice/")]
    [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
    public class Service : System.Web.Services.WebService
    {
        //number of days to generate data for
        private const int NUM_DAYS = 100;

        //names of different valid stocks
        private static readonly string[] STOCKS = { "STOCK1", "STOCK2" };

        private static Dictionary<string, StockInfoHeader[]> dictStockPrices;
        //last access time
        private static long laccess = 0;

        private static Random r = new Random();

        //daemon thread that refreshes stock prices after inactivity
        private static Thread thrDaemon = new Thread(Service.RefreshDaemon);

        //used for locking the stock data
        private static Mutex mtx = new Mutex();

        public Service()
        {
            if (!thrDaemon.IsAlive)
            {
                thrDaemon.Start();
            }
        }        

        [WebMethod]
        public StockInfoHeader GetAll(string stSymbol, int date)
        {
            StockInfoHeader sth;
            StockInfoHeader[] headers;

            sth = new StockInfoHeader();

            ValidateStatus();

            if (dictStockPrices.ContainsKey(stSymbol) && !(date < 0 || date >= NUM_DAYS))
            {
                headers = dictStockPrices[stSymbol];
                sth = headers[date];
            }

            return sth;
        }

        [WebMethod]
        public double GetOpen(string stSymbol, int date)
        {
            return GetAll(stSymbol, date).open;
        }

        [WebMethod]
        public double GetClose(string stSymbol, int date)
        {
            return GetAll(stSymbol, date).close;
        }

        [WebMethod]
        public double GetLow(string stSymbol, int date)
        {
            return GetAll(stSymbol, date).low;
        }

        [WebMethod]
        public double GetHigh(string stSymbol, int date)
        {
            return GetAll(stSymbol, date).high;
        }

        [WebMethod]
        public double[] GetRange(string stSymbol, string infType, int start, int length)
        {
            double[] rgd = null;
            StockInfoHeader[] headers;

            ValidateStatus();

            if (!(start < 0 || length <= 0 || start + length > NUM_DAYS) && dictStockPrices.ContainsKey(stSymbol))
            {
                rgd = new double[length];
                int type = 0;

                if (infType.Equals("LOW"))
                    type = 1;
                else if (infType.Equals("HIGH"))
                    type = 2;
                else if (infType.Equals("OPEN"))
                    type = 3;
                else if (infType.Equals("CLOSE"))
                    type = 4;

                headers = dictStockPrices[stSymbol];
                for (int i = 0; i < length; i++)
                {
                    switch (type)
                    {
                    case 1:
                        rgd[i] = headers[start + i].low;
                        break;
                    case 2:
                        rgd[i] = headers[start + i].high;
                        break;
                    case 3:
                        rgd[i] = headers[start + i].open;
                        break;
                    case 4:
                        rgd[i] = headers[start + i].close;
                        break;
                    }
                }
            }

            return rgd;
        }

        private static void ValidateStatus()
        {
            mtx.WaitOne();

            if (!thrDaemon.IsAlive) thrDaemon.Start();

            if (dictStockPrices == null)
            {
                dictStockPrices = new Dictionary<string, StockInfoHeader[]>();
                foreach (string s in STOCKS)
                    RefreshStockPrices(s);
            }

            laccess = System.DateTime.Now.Ticks;

            mtx.ReleaseMutex();
        }

        // Method for a daemon thread to refresh the stock prices.  Generates an entirely
        // new set of data every 30 seconds, unless that last access was more recent - in
        // that case, it will sleep for 30 seconds and check again.

        private static void RefreshDaemon()
        {
            while (true)
            {
                if (System.DateTime.Now.Ticks - laccess > 300000000)
                {
                    mtx.WaitOne();
                    laccess = System.DateTime.Now.Ticks;
                    dictStockPrices = new Dictionary<string, StockInfoHeader[]>();
                    foreach (string s in STOCKS)
                        RefreshStockPrices(s);
                    mtx.ReleaseMutex();
                }
                Thread.Sleep(30000);
            }
        }

        // Somewhat arbitrarily generates fake stock data.  Makes minor attempts to ensure "believability"
        // in that the stock will behave somehwat normally.  To generate a day of data it takes into account
        // the previous day and ensures there are no extrememe outliers.  It is still entirely possible to
        // have 50 days in a row of increases, but very unlikely

        private static void RefreshStockPrices(string stSymbol)
        {
            StockInfoHeader[] newPrices = new StockInfoHeader[NUM_DAYS];
            StockInfoHeader stihlp;
            double dlow, dhigh, dopen, dclose, dtc;

            if (dictStockPrices.ContainsKey(stSymbol))
            {
                dictStockPrices.Remove(stSymbol);
            }

            dlow = r.NextDouble() * 50.0 + 25.0;
            dhigh = dlow + r.NextDouble() * 10.0 + 2.5;
            dopen = dlow + r.NextDouble() * (dhigh - dlow);
            dclose = dhigh - r.NextDouble() * (dhigh - dlow);
            dtc = 0;

            for (int i = 0; i < NUM_DAYS; i++)
            {
                stihlp = new StockInfoHeader(dopen, dclose, dlow, dhigh);
                newPrices[i] = stihlp;

                dopen = stihlp.close + (stihlp.close - stihlp.open) * (0.5 + r.NextDouble()) * 0.25 + (r.NextDouble() - 0.5) * 10;
                dclose = dopen + (stihlp.close - stihlp.open) * (0.5 + r.NextDouble()) * 0.255 + (r.NextDouble() - 0.5) * 10;

                if (dopen < 10)
                    dopen = r.NextDouble() * 5 + 10;
                if (dclose < 10)
                    dclose = r.NextDouble() * 5 + 10;

                double dtlow, dthigh;
                dtlow = dclose < dopen ? dclose : dopen;
                dthigh = dclose < dopen ? dopen : dclose;

                dlow = dtlow - Math.Pow(r.NextDouble(), 2) * 10;
                dhigh = dthigh + Math.Pow(r.NextDouble(), 2) * 10;

                dtc += dclose - stihlp.close;
            }
            dictStockPrices[stSymbol] = newPrices;
        }
    }
}
