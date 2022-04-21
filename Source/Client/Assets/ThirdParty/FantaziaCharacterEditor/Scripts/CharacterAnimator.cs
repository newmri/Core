using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Spine;
using Spine.Unity;
using System;
using UnityCoreLibrary;

public class CharacterAnimator : MonoBehaviour
{
    //Script responsible for executing the animations of the character

    //SkeletonAnimation is the class that has all the information of the spine animation
    SkeletonAnimation characterSkeleton;

    [HideInInspector]
    public Define.Job MyJob;

    Define.Dir _dir = Define.Dir.RIGHT;

    Dictionary<PlayerAnimations, string> WarriorAnimations = new Dictionary<PlayerAnimations, string>();
    Dictionary<PlayerAnimations, string> ArcherAnimations = new Dictionary<PlayerAnimations, string>();
    Dictionary<PlayerAnimations, string> SorcererAnimations = new Dictionary<PlayerAnimations, string>();
    Dictionary<PlayerAnimations, string> DuelistAnimations = new Dictionary<PlayerAnimations, string>();
    Dictionary<Define.Job, Dictionary<PlayerAnimations, string>> JobsAnimations = new Dictionary<Define.Job, Dictionary<PlayerAnimations, string>>();

    PlayerAnimations AnimationToPlay;
    GearEquipper AccGE;

    public GameObject ArrowPrefab;
    bool IsFireArrow=true;

    void Awake()
    {
        //Assigns characterAnimator to the first child of this transform. Change it to your animation gameobject if you dont want it to be the first transform
        characterSkeleton = transform.GetChild(0).GetComponent<SkeletonAnimation>();

        //Access the class responsible for equipping gears.
        AccGE = GetComponent<GearEquipper>();

        CreateAnimationsDictionary();
    }

    public void AddAnimationEndEvent(Action AnimationEndEvent)
    {
        Spine.AnimationState.TrackEntryDelegate ac = delegate
        {
            AnimationEndEvent();
        };

        characterSkeleton.AnimationState.Complete += ac;
    }

    private void Start()
    {
        //Subscribe the event of the animation to a function called OnEventAnimation
        characterSkeleton.AnimationState.Event += OnEventAnimation;
    }

    void OnEventAnimation(TrackEntry trackEntry, Spine.Event e)
    {
        //This function only contains one event ("OnArrowLeftBow"). You can add whatever event you want the is available in the asset. Read the documentation provided to know the available events
        if (e.Data.Name == "OnArrowLeftBow")
        {
            //Shoots a real arrow when the arrow in the animation leaves the bow

            if (IsFireArrow == false)
                return;

            Vector3 ArrowStartingPosition = Vector3.zero;

            if (trackEntry.Animation.ToString() == "Shoot1")
            {
                ArrowStartingPosition = transform.Find("ArrowsFirePoints").Find("FirePoint_Shoot1").position;
            }
            else if (trackEntry.Animation.ToString() == "Shoot2")
            {
                ArrowStartingPosition = transform.Find("ArrowsFirePoints").Find("FirePoint_Shoot2").position;
            }
            else if (trackEntry.Animation.ToString() == "Shoot3")
            {
                ArrowStartingPosition = transform.Find("ArrowsFirePoints").Find("FirePoint_Shoot3").position;
            }

            //Managers.Creature.MyPlayer.SendUseSkill();

           // Vector2Int cellPos = new Vector2Int((int)ArrowStartingPosition.x, (int)ArrowStartingPosition.y);
            //Debug.Log(cellPos.x + " " + cellPos.y);

            //CoreManagers.Obj.Add(ArrowPrefab, cellPos, 100);
        }
        else if(e.Data.Name == "OnDamaging" || e.Data.Name == "OnFinishingCasting")
        {
            //Managers.Creature.MyPlayer.SendUseSkill();
        }

    }

    public void FireArrow_Archer()
    {
        IsFireArrow = !IsFireArrow;
    }
    

    void CreateAnimationsDictionary()
    { 
        //Fills the dicitionaries with all the animations of each job

        WarriorAnimations.Add(PlayerAnimations.ATTACK_1, "Attack1");
        WarriorAnimations.Add(PlayerAnimations.Attack2, "Attack2");
        WarriorAnimations.Add(PlayerAnimations.IDLE, "Idle");
        WarriorAnimations.Add(PlayerAnimations.WALK, "Walk");
        WarriorAnimations.Add(PlayerAnimations.RUN, "Run");
        WarriorAnimations.Add(PlayerAnimations.FullJump, "Jump");
        WarriorAnimations.Add(PlayerAnimations.Jump1, "Jump1");
        WarriorAnimations.Add(PlayerAnimations.Jump2, "Jump2");
        WarriorAnimations.Add(PlayerAnimations.Jump3, "Jump3");
        WarriorAnimations.Add(PlayerAnimations.Buff, "Buff");
        WarriorAnimations.Add(PlayerAnimations.HIT, "Hurt");
        WarriorAnimations.Add(PlayerAnimations.Special, "Defence");
        WarriorAnimations.Add(PlayerAnimations.DEAD, "Death");

        ArcherAnimations.Add(PlayerAnimations.ATTACK_1, "Shoot1");
        ArcherAnimations.Add(PlayerAnimations.Attack2, "Shoot2");
        ArcherAnimations.Add(PlayerAnimations.IDLE, "Idle ARCHER");
        ArcherAnimations.Add(PlayerAnimations.WALK, "Walk");
        ArcherAnimations.Add(PlayerAnimations.RUN, "Run ARCHER");
        ArcherAnimations.Add(PlayerAnimations.FullJump, "Jump");
        ArcherAnimations.Add(PlayerAnimations.Jump1, "Jump1 ARCHER");
        ArcherAnimations.Add(PlayerAnimations.Jump2, "Jump2");
        ArcherAnimations.Add(PlayerAnimations.Jump3, "Jump3 ARCHER");
        ArcherAnimations.Add(PlayerAnimations.Buff, "Buff");
        ArcherAnimations.Add(PlayerAnimations.HIT, "Hurt");
        ArcherAnimations.Add(PlayerAnimations.Special, "Shoot3");
        ArcherAnimations.Add(PlayerAnimations.DEAD, "Death");

        SorcererAnimations.Add(PlayerAnimations.ATTACK_1, "Cast2");
        SorcererAnimations.Add(PlayerAnimations.Attack2, "Cast2");
        SorcererAnimations.Add(PlayerAnimations.IDLE, "Idle");
        SorcererAnimations.Add(PlayerAnimations.WALK, "Walk");
        SorcererAnimations.Add(PlayerAnimations.RUN, "Fly");
        SorcererAnimations.Add(PlayerAnimations.FullJump, "Jump");
        SorcererAnimations.Add(PlayerAnimations.Jump1, "Jump1");
        SorcererAnimations.Add(PlayerAnimations.Jump2, "Jump2");
        SorcererAnimations.Add(PlayerAnimations.Jump3, "Jump3");
        SorcererAnimations.Add(PlayerAnimations.Buff, "Buff");
        SorcererAnimations.Add(PlayerAnimations.HIT, "Hurt");
        SorcererAnimations.Add(PlayerAnimations.Special, "Cast3");
        SorcererAnimations.Add(PlayerAnimations.DEAD, "Death");

        DuelistAnimations.Add(PlayerAnimations.ATTACK_1, "Attack 1 DUELIST");
        DuelistAnimations.Add(PlayerAnimations.Attack2, "Attack 2 DUELIST");
        DuelistAnimations.Add(PlayerAnimations.IDLE, "Idle");
        DuelistAnimations.Add(PlayerAnimations.WALK, "Walk");
        DuelistAnimations.Add(PlayerAnimations.RUN, "Run DUELIST");
        DuelistAnimations.Add(PlayerAnimations.FullJump, "Jump");
        DuelistAnimations.Add(PlayerAnimations.Jump1, "Jump1");
        DuelistAnimations.Add(PlayerAnimations.Jump2, "Jump2");
        DuelistAnimations.Add(PlayerAnimations.Jump3, "Jump3");
        DuelistAnimations.Add(PlayerAnimations.Buff, "Buff");
        DuelistAnimations.Add(PlayerAnimations.HIT, "Hurt");
        DuelistAnimations.Add(PlayerAnimations.Special, "Attack 3 DUELIST");
        DuelistAnimations.Add(PlayerAnimations.DEAD, "Death");

        JobsAnimations.Add(Define.Job.ARCHER, ArcherAnimations);
        JobsAnimations.Add(Define.Job.WARRIOR, WarriorAnimations);
        JobsAnimations.Add(Define.Job.SORCERER, SorcererAnimations);
        JobsAnimations.Add(Define.Job.DUELIST, DuelistAnimations);
    }

    //Function called when the job is changed 
    public void JobChanged(Define.Job NewJob)
    {
        Define.Job OldJob = MyJob;
        MyJob = NewJob;
        if (JobsAnimations[MyJob][AnimationToPlay] != JobsAnimations[OldJob][AnimationToPlay])
        {
            AnimationManager();
        }

    }

    //Takes a string AnimationString which is the name of the animation and assigns it to AnimationToPlay
    public void ChangeAnimation(Define.ObjectState state, Define.Dir dir, SkillAnimationType skillType)
    {
        if (_dir != dir)
        {
            if (Define.Dir.RIGHT == dir)
            {
                transform.localScale = new Vector3(1.0f, 1.0f, 1.0f);
            }
            else
            {
                transform.localScale = new Vector3(-1.0f, 1.0f, 1.0f);
            }
        }

        _dir = dir;

        if (state == Define.ObjectState.SKILL)
            AnimationToPlay = (PlayerAnimations)Enum.Parse(typeof(PlayerAnimations), skillType.ToString());
        else
            AnimationToPlay = (PlayerAnimations)Enum.Parse(typeof(PlayerAnimations), state.ToString());

        AnimationManager();
    }

    //Runs the required animation using SetAnimation spine function
    void AnimationManager()
    {
        bool IsLoop = true;

        switch (AnimationToPlay)
        {
            case PlayerAnimations.ATTACK_1:
            case PlayerAnimations.DEAD:
                IsLoop = false;
                break;
            default:
                break;
        }

        characterSkeleton.AnimationState.SetAnimation(0, JobsAnimations[AccGE.Job][AnimationToPlay], IsLoop);
    }

}
public enum PlayerAnimations
{
    IDLE, WALK, ATTACK_1, DEAD, FullJump,Jump1, Jump2, Jump3, HIT, RUN, Attack2, Special, Buff
}
