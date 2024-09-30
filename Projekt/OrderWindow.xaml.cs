using Projekt;
using System;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Projekt
{
    public partial class OrderWindow : Window
    {
        private ObservableCollection<Product> _selectedProducts;
        public ObservableCollection<Order> Orders { get; set; }
        public OrderWindow(ObservableCollection<Product> products)
        {
            InitializeComponent();
            _selectedProducts = new ObservableCollection<Product>(products);
            Orders = new ObservableCollection<Order>{};
            ProductsListBox.ItemsSource = _selectedProducts;
        }

        private void OrderButton_Click(object sender, RoutedEventArgs e)
        {
            string orderHelper = "";
            double totalPrice = 0;
            SummaryLabel.Content = "";
            foreach (Product product in ProductsListBox.SelectedItems)
            {
                ListBoxItem listBoxItem = ProductsListBox.ItemContainerGenerator.ContainerFromItem(product) as ListBoxItem;
                if (listBoxItem != null)
                {
                    TextBox quantityTextBox = FindVisualChild<TextBox>(listBoxItem);
                    if (quantityTextBox != null && int.TryParse(quantityTextBox.Text, out int quantity) && quantity > 0)
                    {
                        double productPrice = product.Price * quantity;
                        totalPrice += productPrice;

                        SummaryLabel.Content += $"{product.Name}: {quantity} x {product.Price:C} = {productPrice:C}\n";
                        orderHelper += $"{quantity} x {product.Name}\n";
                    }
                }
            }
            Order ord = new Order(DateTime.Now, orderHelper, totalPrice);
            Orders.Add(ord);
            TotalPriceLabel.Content = totalPrice.ToString("C");
        }

        private T FindVisualChild<T>(DependencyObject parent) where T : DependencyObject
        {
            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(parent); i++)
            {
                DependencyObject child = VisualTreeHelper.GetChild(parent, i);
                if (child != null && child is T)
                    return (T)child;
                else
                {
                    T childOfChild = FindVisualChild<T>(child);
                    if (childOfChild != null)
                        return childOfChild;
                }
            }
            return null;
        }

        private void OrderProductsButton_Click(object sender, RoutedEventArgs e)
        {
            OrderTrackingWindow orderTrackingWindow = new OrderTrackingWindow(Orders);
            orderTrackingWindow.ShowDialog();
        }
    }
}