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
            Title,
            Login,
            Lobby,
            Game,
            Result,
        }

        public enum UIEvent
        {
            Enter,
            Exit,
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
