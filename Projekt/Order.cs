using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Projekt
{
    public class Order
    {
        static int nextId;
        public int OrderId { get; private set; }
        public DateTime Date { get; set; }
        public string Products { get; set; }
        public double TotalPrice { get; set; }

        // Konstruktor
        public Order(DateTime date, string products, double totalPrice)
        {
            OrderId = Interlocked.Increment(ref nextId);
            Date = date;
            Products = products;
            TotalPrice = totalPrice;
        }
    }
}
