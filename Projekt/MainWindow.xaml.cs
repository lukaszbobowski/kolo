using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace Projekt
{
    public partial class MainWindow : Window
    {
        public ObservableCollection<Product> Products { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            Products = new ObservableCollection<Product>
            {
                new Product { Name = "Chleb", Price = 2.5 },
                new Product { Name = "Bułka", Price = 1.0 },
                new Product { Name = "Pączek", Price = 1.5 }
            };
            DataContext = this;
        }

        private void AddProductButton_Click(object sender, RoutedEventArgs e)
        {
            if(double.TryParse(NewProductPriceTextBox.Text, out double price))
            {
                Products.Add(new Product { Name = NewProductNameTextBox.Text, Price = price });
                NewProductNameTextBox.Text = "";
                NewProductPriceTextBox.Text = "";
            }
        }

        private void ExitMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void OrderProductsButton_Click(object sender, RoutedEventArgs e)
        {
            OrderWindow orderWindow = new OrderWindow(Products);
            orderWindow.ShowDialog();
        }
    }

    public class Product : INotifyPropertyChanged
    {
        private string _name;
        public string Name
        {
            get { return _name; }
            set
            {
                _name = value;
                OnPropertyChanged("Name");
            }
        }

        private double _price;
        public double Price
        {
            get { return _price; }
            set
            {
                _price = value;
                OnPropertyChanged("Price");
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }


}