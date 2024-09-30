﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Projekt
{
    public partial class OrderTrackingWindow : Window
    {
        private ObservableCollection<Order> _orders;

        public OrderTrackingWindow(ObservableCollection<Order> orders)
        {
            InitializeComponent();
            _orders = orders;
            OrdersListView.ItemsSource = _orders;
        }
    }
}
