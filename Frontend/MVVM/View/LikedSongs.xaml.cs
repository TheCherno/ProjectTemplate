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
        [DllImport("Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateAudioManager();
        [DllImport("Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AudioManager_PlayPog(IntPtr audio, [MarshalAs(UnmanagedType.LPStr)] string name);
        [DllImport("Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyAudioManager(IntPtr audio);
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

        private async void songsListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Console.WriteLine("Trying");
            IntPtr audioManager = CreateAudioManager();
            Console.WriteLine(audioManager);
            await Task.Run(() => { AudioManager_PlayPog(audioManager, "20:15"); });
            DestroyAudioManager(audioManager);
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
