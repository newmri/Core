using UnityEngine;

namespace UnityCoreLibrary
{
    public class Define
    {
        public enum Sound
        {
            BGM,
            Effect,
            MaxCount,
        }
        public enum Scene
        {
            Unknown,
            Login,
            Lobby,
            Game,
            Result,
        }

        public enum UIEvent
        {
            Click,
            Drag,
        }

        public enum CameraMode
        { QuarterView };

        public enum MouseEvent
        {
            ButtonDown,
            ButtonIng,
            ButtonUp,
        }
    }

}
