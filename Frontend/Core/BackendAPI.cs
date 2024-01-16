using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Frontend.Core
{
    internal class BackendAPI
    {
        [DllImport("Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr CreateAudioManager();

        public AudioManager createAudioManager()
        {
            IntPtr audioManager = CreateAudioManager();
            return new AudioManager(audioManager);
        }
    }

    internal class AudioManager
    {
        [DllImport("Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AudioManager_PlayPog(IntPtr audio, [MarshalAs(UnmanagedType.LPStr)] string name);
        [DllImport("Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyAudioManager(IntPtr audio);
        private IntPtr pointer;

        //Obtain the required pointer from the CreateAudioManager method exposed by the Backend.dll
        public AudioManager(IntPtr audioManagerPointer) { 
            pointer = audioManagerPointer; 
        }
    }
}
