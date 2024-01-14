using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Frontend.MVVM.View
{
    /// <summary>
    /// Interaction logic for LikedSongs.xaml
    /// </summary>
    public partial class LikedSongs : UserControl
    {
        [DllImport("Backend.dll", CallingConvention = CallingConvention.Cdecl, CharSet= CharSet.Ansi)]
        private static extern void playSong(string songName);
        public LikedSongs()
        {
            InitializeComponent();

            List<Song> list = new List<Song>
            {
                new Song{ Artist = "TJ_beastoby", Title = "20:15", Duration = "3:32", ImagePath = "C:\\Users\\Kreiseljustus\\Documents\\ab67616d0000b273ed1490bec97b20e519d083ac.jpg"},
                new Song{ Artist = "Eminem",Title = "Lose Yourself", Duration = "3:32", ImagePath = "C:\\Users\\Kreiseljustus\\Documents\\ab67616d0000b273ed1490bec97b20e519d083ac.jpg"},
                new Song{ Artist = "Eminem",Title = "Lose Yourself", Duration = "3:32", ImagePath = "C:\\Users\\Kreiseljustus\\Documents\\ab67616d0000b273ed1490bec97b20e519d083ac.jpg"},
                new Song{ Artist = "Eminem",Title = "Lose Yourself", Duration = "3:32", ImagePath = "C:\\Users\\Kreiseljustus\\Documents\\ab67616d0000b273ed1490bec97b20e519d083ac.jpg"}
            };

            songsListBox.ItemsSource = list;
        }

        private void songsListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if(songsListBox.SelectedItem != null)
            {
                Song selectedSong = (Song)songsListBox.SelectedItem;
                //StringBuilder titleBuilder = new StringBuilder(selectedSong.Title);

                Task.Run(() => playSong("20:15"));
                //songsListBox.SelectedItem = null;
            } else
            {
                Console.WriteLine("Null");
            }
        }
    }

    public class Song
    {
        public string Title { get; set; }
        public string Duration { get; set; }
        public string ImagePath { get; set; }
        public string Artist { get; set; }
    }
}
