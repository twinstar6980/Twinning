#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Data;
using Microsoft.UI.Xaml.Controls.Primitives;
using Microsoft.UI.Xaml.Input;
using Windows.Globalization.NumberFormatting;
using Microsoft.UI.Xaml.Media.Imaging;
using DecimalFormatter = Windows.Globalization.NumberFormatting.DecimalFormatter;
using IncrementNumberRounder = Windows.Globalization.NumberFormatting.IncrementNumberRounder;
using FluentIconGlyph = Helper.CustomControl.FluentIconGlyph;

namespace Helper.Module.AnimationViewer {

	public sealed partial class MainPage : Page {

		#region life

		public MainPage (
		) {
			this.InitializeComponent();
			this.Controller = new MainPageController() { View = this };
			this.uSprite.HoldEnd = true;
			this.uSprite.Repeat = true;
			this.uWorkingSpriteFrameProgress.AddHandler(UIElement.PointerPressedEvent, new PointerEventHandler(this.Controller.uWorkingSpriteFrameProgress_OnPointerPressed), true);
			this.uWorkingSpriteFrameProgress.AddHandler(UIElement.PointerReleasedEvent, new PointerEventHandler(this.Controller.uWorkingSpriteFrameProgress_OnPointerReleased), true);
		}

		// ----------------

		private MainPageController Controller { get; }

		#endregion

	}

	public class MainPageController : CustomController {

		#region data

		public required MainPage View;

		// ----------------

		public Floating StageScale { get; set; } = 1.0;

		public Floating StagePositionX { get; set; } = 0.0;

		public Floating StagePositionY { get; set; } = 0.0;

		// ----------------

		public Boolean AutomaticPlay { get; set; } = true;

		public Boolean RepeatPlay { get; set; } = true;

		// ----------------

		public String? AnimationFile { get; set; } = null;

		public String? ImageDirectory { get; set; } = null;

		// ----------------

		public AnimationModel.Animation? Animation { get; set; } = null;

		public List<BitmapSource?>? ImageSource { get; set; } = null;

		public List<Boolean>? ImageFilter { get; set; } = null;

		public List<Boolean>? SpriteFilter { get; set; } = null;

		// ----------------

		public Size? WorkingSpriteIndex { get; set; } = null;

		public Floating? WorkingSpriteFrameRate { get; set; } = null;

		public AnimationHelper.FrameRange? WorkingSpriteFrameRange { get; set; } = null;

		public List<Tuple<String, AnimationHelper.FrameRange>>? WorkingSpriteFrameRangeLabelInformation { get; set; } = null;

		public Boolean? WorkingSpritePaused { get; set; } = null;

		// ----------------

		public Boolean ShowSpriteBoundary { get; set; } = false;

		public List<String>? PlantCustomLayerName { get; set; } = null;

		public List<String>? ZombieStateLayerName { get; set; } = null;

		public List<String>? ZombieGroundSwatchLayerName { get; set; } = null;

		#endregion

		#region load & work

		[MemberNotNullWhen(true, nameof(MainPageController.AnimationFile))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageDirectory))]
		[MemberNotNullWhen(true, nameof(MainPageController.Animation))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageSource))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.SpriteFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.PlantCustomLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieStateLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieGroundSwatchLayerName))]
		public Boolean Loaded {
			get {
				return this.Animation != null;
			}
		}

		[MemberNotNullWhen(true, nameof(MainPageController.AnimationFile))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageDirectory))]
		[MemberNotNullWhen(true, nameof(MainPageController.Animation))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageSource))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.SpriteFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.PlantCustomLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieStateLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieGroundSwatchLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.WorkingSpriteIndex))]
		[MemberNotNullWhen(true, nameof(MainPageController.WorkingSpriteFrameRate))]
		[MemberNotNullWhen(true, nameof(MainPageController.WorkingSpriteFrameRange))]
		[MemberNotNullWhen(true, nameof(MainPageController.WorkingSpriteFrameRangeLabelInformation))]
		[MemberNotNullWhen(true, nameof(MainPageController.WorkingSpritePaused))]
		public Boolean Working {
			get {
				return this.Loaded && this.WorkingSpriteIndex != null;
			}
		}

		// ----------------

		public async Task Load (
			String animationFile,
			String imageDirectory
		) {
			Debug.Assert(!this.Loaded && !this.Working);
			this.AnimationFile = animationFile;
			this.ImageDirectory = imageDirectory;
			this.Animation = await AnimationHelper.LoadAnimation(animationFile);
			this.ImageSource = await AnimationHelper.LoadImageSource(imageDirectory, this.Animation);
			this.ImageFilter = Enumerable.Range(0, this.Animation.Image.Count).Select(i => false).ToList();
			this.SpriteFilter = Enumerable.Range(0, this.Animation.Sprite.Count).Select(i => false).ToList();
			this.PlantCustomLayerName = (from item in this.Animation.Sprite where item.Name.StartsWith("custom_") select item.Name).ToList();
			this.ZombieStateLayerName = (from item in this.Animation.Sprite where item.Name == "ink" || item.Name == "butter" select item.Name).ToList();
			this.ZombieGroundSwatchLayerName = (from item in this.Animation.Sprite where item.Name == "ground_swatch" || item.Name == "ground_swatch_plane" select item.Name).ToList();
			this.NotifyPropertyChanged(
				nameof(this.uAnimationFile_IsEnabled),
				nameof(this.uAnimationFile_Text),
				nameof(this.uSelectAnimationFile_IsEnabled),
				nameof(this.uImageDirectory_IsEnabled),
				nameof(this.uImageDirectory_Text),
				nameof(this.uSelectImageDirectory_IsEnabled),
				nameof(this.uClearSource_IsEnabled),
				nameof(this.uImageList_IsEnabled),
				nameof(this.uImageList_ItemsSource),
				nameof(this.uSpriteList_IsEnabled),
				nameof(this.uSpriteList_ItemsSource),
				nameof(this.uMainSpriteList_IsEnabled),
				nameof(this.uMainSpriteList_ItemsSource)
			);
			this.View.uImageList.SelectAll();
			this.View.uSpriteList.SelectAll();
			if (this.Animation.MainSprite != null) {
				this.View.uMainSpriteList.SelectAll();
			}
			this.NotifyPropertyChanged(
				nameof(this.uPlantCustomLayerIcon_Opacity),
				nameof(this.uPlantCustomLayer_IsEnabled),
				nameof(this.uPlantCustomLayer_ItemsSource),
				nameof(this.uPlantCustomLayer_SelectedValue),
				nameof(this.uZombieStateLayerIcon_Opacity),
				nameof(this.uZombieStateLayer_IsEnabled),
				nameof(this.uZombieStateLayer_ItemsSource),
				nameof(this.uZombieStateLayer_SelectedValue),
				nameof(this.uZombieGroundSwatchLayerIcon_Opacity),
				nameof(this.uZombieGroundSwatchLayer_IsEnabled),
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			return;
		}

		public async Task Unload (
		) {
			Debug.Assert(this.Loaded && !this.Working);
			this.View.uImageList.DeselectRange(new ItemIndexRange(0, (USize)this.Animation.Image.Count));
			this.View.uSpriteList.DeselectRange(new ItemIndexRange(0, (USize)this.Animation.Sprite.Count));
			if (this.Animation.MainSprite != null) {
				this.View.uMainSpriteList.DeselectRange(new ItemIndexRange(0, 1));
			}
			this.AnimationFile = null;
			this.ImageDirectory = null;
			this.Animation = null;
			this.ImageSource = null;
			this.ImageFilter = null;
			this.SpriteFilter = null;
			this.PlantCustomLayerName = null;
			this.ZombieStateLayerName = null;
			this.ZombieGroundSwatchLayerName = null;
			this.NotifyPropertyChanged(
				nameof(this.uAnimationFile_IsEnabled),
				nameof(this.uAnimationFile_Text),
				nameof(this.uSelectAnimationFile_IsEnabled),
				nameof(this.uImageDirectory_IsEnabled),
				nameof(this.uImageDirectory_Text),
				nameof(this.uSelectImageDirectory_IsEnabled),
				nameof(this.uClearSource_IsEnabled),
				nameof(this.uImageList_IsEnabled),
				nameof(this.uImageList_ItemsSource),
				nameof(this.uSpriteList_IsEnabled),
				nameof(this.uSpriteList_ItemsSource),
				nameof(this.uMainSpriteList_IsEnabled),
				nameof(this.uMainSpriteList_ItemsSource),
				nameof(this.uPlantCustomLayerIcon_Opacity),
				nameof(this.uPlantCustomLayer_IsEnabled),
				nameof(this.uPlantCustomLayer_ItemsSource),
				nameof(this.uPlantCustomLayer_SelectedValue),
				nameof(this.uZombieStateLayerIcon_Opacity),
				nameof(this.uZombieStateLayer_IsEnabled),
				nameof(this.uZombieStateLayer_ItemsSource),
				nameof(this.uZombieStateLayer_SelectedValue),
				nameof(this.uZombieGroundSwatchLayerIcon_Opacity),
				nameof(this.uZombieGroundSwatchLayer_IsEnabled),
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			return;
		}

		// ----------------

		public async Task LoadWorkingSprite (
			Size index
		) {
			Debug.Assert(this.Loaded && !this.Working);
			var workingSprite = AnimationHelper.SelectSprite(this.Animation, index);
			this.WorkingSpriteIndex = index;
			this.WorkingSpriteFrameRate = workingSprite.FrameRate;
			this.WorkingSpriteFrameRange = new AnimationHelper.FrameRange() { Start = 0, Duration = workingSprite.Frame.Count };
			this.WorkingSpriteFrameRangeLabelInformation = new List<Tuple<String, AnimationHelper.FrameRange>>();
			var currentFrameLabel = new List<Tuple<String, Size>>();
			for (var frameIndex = 0; frameIndex < workingSprite.Frame.Count; ++frameIndex) {
				var frame = workingSprite.Frame[frameIndex];
				if (frame.Label != null) {
					currentFrameLabel.Add(new Tuple<String, Size>(frame.Label, frameIndex));
				}
				if (frame.Stop) {
					foreach (var item in currentFrameLabel) {
						this.WorkingSpriteFrameRangeLabelInformation.Add(new Tuple<String, AnimationHelper.FrameRange>(item.Item1, new AnimationHelper.FrameRange() {
							Start = item.Item2,
							Duration = frameIndex - item.Item2 + 1,
						}));
					}
					currentFrameLabel.Clear();
				}
			}
			this.WorkingSpritePaused = !this.AutomaticPlay;
			this.View.uSprite.Load(this.Animation, this.ImageSource, this.ImageFilter, this.SpriteFilter, index);
			Debug.Assert(this.View.uSprite.Loaded);
			this.View.uSprite.Speed = workingSprite.FrameRate;
			var sliderAnimation = new ObjectAnimationUsingKeyFrames();
			for (var i = 0; i < this.WorkingSpriteFrameRange.Duration; ++i) {
				sliderAnimation.KeyFrames.Add(
					new DiscreteObjectKeyFrame() {
						KeyTime = KeyTime.FromTimeSpan(TimeSpan.FromSeconds(i)),
						Value = (Floating)(i + 1),
					}
				);
			}
			Storyboard.SetTargetProperty(sliderAnimation, "Value");
			var sliderStoryboard = new Storyboard() {
				Duration = new Duration(TimeSpan.FromSeconds(this.WorkingSpriteFrameRange.Duration)),
				RepeatBehavior = RepeatBehavior.Forever,
				Children = {
					sliderAnimation,
				},
			};
			Storyboard.SetTarget(sliderStoryboard, this.View.uWorkingSpriteFrameProgress);
			this.View.uSprite.Storyboard.Children.Add(sliderStoryboard);
			this.View.uSprite.Storyboard.Completed += (
				sender,
				o
			) => {
				if (!this.RepeatPlay) {
					this.View.uSprite.State = SpriteControl.StateType.Paused;
					this.WorkingSpritePaused = true;
					this.NotifyPropertyChanged(
						nameof(this.uWorkingSpritePauseIcon_Glyph)
					);
				}
			};
			this.View.uSprite.State = !this.AutomaticPlay ? SpriteControl.StateType.Paused : SpriteControl.StateType.Playing;
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpriteFrameRateIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRate_IsEnabled),
				nameof(this.uWorkingSpriteFrameRate_Value),
				nameof(this.uWorkingSpriteFrameRangeIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRangeBegin_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeBegin_Minimum),
				nameof(this.uWorkingSpriteFrameRangeBegin_Maximum),
				nameof(this.uWorkingSpriteFrameRangeBegin_Value),
				nameof(this.uWorkingSpriteFrameRangeEnd_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeEnd_Minimum),
				nameof(this.uWorkingSpriteFrameRangeEnd_Maximum),
				nameof(this.uWorkingSpriteFrameRangeEnd_Value),
				nameof(this.uWorkingSpriteFrameRangeLabelIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRangeLabel_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeLabel_ItemsSource),
				nameof(this.uWorkingSpriteFrameRangeLabel_SelectedValue),
				nameof(this.uWorkingSpriteFrameProgress_IsEnabled),
				nameof(this.uWorkingSpriteFrameProgress_Minimum),
				nameof(this.uWorkingSpriteFrameProgress_Maximum),
				nameof(this.uWorkingSpritePause_IsEnabled),
				nameof(this.uWorkingSpritePauseIcon_Glyph),
				nameof(this.uWorkingSpritePrev_IsEnabled),
				nameof(this.uWorkingSpriteNext_IsEnabled)
			);
			foreach (var item in (List<SpriteItemController>)this.View.uSpriteList.ItemsSource) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			foreach (var item in (List<MainSpriteItemController>)this.View.uMainSpriteList.ItemsSource) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			return;
		}

		public async Task UnloadWorkingSprite (
		) {
			Debug.Assert(this.Loaded && this.Working);
			this.WorkingSpriteIndex = null;
			this.WorkingSpriteFrameRate = null;
			this.WorkingSpriteFrameRange = null;
			this.WorkingSpriteFrameRangeLabelInformation = null;
			this.WorkingSpritePaused = null;
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpriteFrameRateIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRate_IsEnabled),
				nameof(this.uWorkingSpriteFrameRate_Value),
				nameof(this.uWorkingSpriteFrameRangeIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRangeBegin_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeBegin_Minimum),
				nameof(this.uWorkingSpriteFrameRangeBegin_Maximum),
				nameof(this.uWorkingSpriteFrameRangeBegin_Value),
				nameof(this.uWorkingSpriteFrameRangeEnd_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeEnd_Minimum),
				nameof(this.uWorkingSpriteFrameRangeEnd_Maximum),
				nameof(this.uWorkingSpriteFrameRangeEnd_Value),
				nameof(this.uWorkingSpriteFrameRangeLabelIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRangeLabel_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeLabel_ItemsSource),
				nameof(this.uWorkingSpriteFrameRangeLabel_SelectedValue),
				nameof(this.uWorkingSpriteFrameProgress_IsEnabled),
				nameof(this.uWorkingSpriteFrameProgress_Minimum),
				nameof(this.uWorkingSpriteFrameProgress_Maximum),
				nameof(this.uWorkingSpritePause_IsEnabled),
				nameof(this.uWorkingSpritePauseIcon_Glyph),
				nameof(this.uWorkingSpritePrev_IsEnabled),
				nameof(this.uWorkingSpriteNext_IsEnabled)
			);
			foreach (var item in (List<SpriteItemController>)this.View.uSpriteList.ItemsSource) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			foreach (var item in (List<MainSpriteItemController>)this.View.uMainSpriteList.ItemsSource) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			this.View.uSprite.Unload();
			return;
		}

		public async Task UpdateWorkingSpriteFrameRange (
			AnimationHelper.FrameRange frameRange
		) {
			Debug.WriteLine($"ren {frameRange.Start}, {frameRange.Duration}");
			Debug.Assert(this.Loaded && this.Working);
			this.WorkingSpriteFrameRange = frameRange;
			this.View.uSprite.FrameRange = frameRange;
			this.View.uSprite.CurrentTime = TimeSpan.FromSeconds(frameRange.Start);
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpriteFrameRangeBegin_Value),
				nameof(this.uWorkingSpriteFrameRangeEnd_Value),
				nameof(this.uWorkingSpriteFrameRangeLabel_SelectedValue),
				nameof(this.uWorkingSpriteFrameProgress_Minimum),
				nameof(this.uWorkingSpriteFrameProgress_Maximum),
				nameof(this.uWorkingSpritePauseIcon_Glyph)
			);
			return;
		}

		#endregion

		#region stage

		public String uStagePositionX_Text {
			get {
				return $"{this.StagePositionX:F0}";
			}
		}

		// ----------------

		public String uStagePositionY_Text {
			get {
				return $"{this.StagePositionY:F0}";
			}
		}

		// ----------------

		public String uStageScale_Text {
			get {
				return $"{this.StageScale * 100:F0} %";
			}
		}

		// ----------------

		public String uStageScaleIcon_Glyph {
			get {
				var valueLose = (Size)Math.Round(this.StageScale * 100.0);
				if (valueLose < 100) {
					return FluentIconGlyph.BackToWindow;
				} else if (valueLose > 100) {
					return FluentIconGlyph.FullScreen;
				} else {
					return FluentIconGlyph.Reshare;
				}
			}
		}

		// ----------------

		public async void uStage_OnViewChanged (
			Object sender,
			Object e
		) {
			var senderX = (ScrollViewer)sender;
			var newScale = senderX.ZoomFactor;
			var newPositionX = (senderX.HorizontalOffset + senderX.ActualWidth / 2) / senderX.ZoomFactor;
			var newPositionY = (senderX.VerticalOffset + senderX.ActualHeight / 2) / senderX.ZoomFactor;
			this.StageScale = newScale;
			this.StagePositionX = newPositionX;
			this.StagePositionY = newPositionY;
			this.NotifyPropertyChanged(
				nameof(this.uStagePositionX_Text),
				nameof(this.uStagePositionY_Text),
				nameof(this.uStageScale_Text),
				nameof(this.uStageScaleIcon_Glyph)
			);
		}

		#endregion

		#region setting

		public Boolean uAutomaticPlay_IsChecked {
			get {
				return this.AutomaticPlay;
			}
		}

		public async void uAutomaticPlay_OnChecked (
			Object          sender,
			RoutedEventArgs e
		) {
			var senderX = (ToggleButton)e.OriginalSource;
			var newValue = senderX.IsChecked!.Value;
			this.AutomaticPlay = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uAutomaticPlay_IsChecked)
			);
		}

		// ----------------

		public Boolean uRepeatPlay_IsChecked {
			get {
				return this.RepeatPlay;
			}
		}

		public async void uRepeatPlay_OnChecked (
			Object          sender,
			RoutedEventArgs e
		) {
			var senderX = (ToggleButton)e.OriginalSource;
			var newValue = senderX.IsChecked!.Value;
			this.RepeatPlay = newValue;
			this.View.uSprite.Repeat = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uRepeatPlay_IsChecked)
			);
		}

		#endregion

		#region source

		public Boolean uClearSource_IsEnabled {
			get {
				return this.Loaded;
			}
		}

		public async void uClearSource_OnClick (
			Object          sender,
			RoutedEventArgs e
		) {
			Debug.Assert(this.Loaded);
			if (this.Working) {
				await this.UnloadWorkingSprite();
			}
			await this.Unload();
		}

		// ----------------

		public Boolean uAnimationFile_IsEnabled {
			get {
				return this.Loaded;
			}
		}

		public String uAnimationFile_Text {
			get {
				return !this.Loaded
					? ""
					: this.AnimationFile;
			}
		}

		// ----------------

		public Boolean uSelectAnimationFile_IsEnabled {
			get {
				return true;
			}
		}

		public async void uSelectAnimationFile_OnClick (
			Object          sender,
			RoutedEventArgs e
		) {
			var isPlaying = this.Working && this.View.uSprite.State == SpriteControl.StateType.Playing;
			if (isPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
			}
			var animationFile = await StorageHelper.PickFile(WindowHelper.GetWindowForElement(this.View) ?? throw new Exception(), ".json");
			if (animationFile != null) {
				var imageDirectory = Path.GetDirectoryName(animationFile) ?? throw new Exception();
				if (this.Loaded) {
					if (this.Working) {
						await this.UnloadWorkingSprite();
					}
					await this.Unload();
				}
				await this.Load(animationFile, imageDirectory);
			} else {
				if (isPlaying) {
					this.View.uSprite.State = SpriteControl.StateType.Playing;
				}
			}
		}

		// ----------------

		public Boolean uImageDirectory_IsEnabled {
			get {
				return this.Loaded;
			}
		}

		public String uImageDirectory_Text {
			get {
				return !this.Loaded
					? ""
					: this.ImageDirectory;
			}
		}

		// ----------------

		public Boolean uSelectImageDirectory_IsEnabled {
			get {
				return this.Loaded;
			}
		}

		public async void uSelectImageDirectory_OnClick (
			Object          sender,
			RoutedEventArgs e
		) {
			Debug.Assert(this.Loaded);
			var isPlaying = this.Working && this.View.uSprite.State == SpriteControl.StateType.Playing;
			if (isPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
			}
			var imageDirectory = await StorageHelper.PickDirectory(WindowHelper.GetWindowForElement(this.View) ?? throw new Exception());
			if (imageDirectory != null) {
				var animationFile = this.AnimationFile;
				if (this.Loaded) {
					if (this.Working) {
						await this.UnloadWorkingSprite();
					}
					await this.Unload();
				}
				await this.Load(animationFile, imageDirectory);
			} else {
				if (isPlaying) {
					this.View.uSprite.State = SpriteControl.StateType.Playing;
				}
			}
		}

		#endregion

		#region image & sprite

		public Boolean uImageList_IsEnabled {
			get {
				return this.Loaded;
			}
		}

		public List<ImageItemController> uImageList_ItemsSource {
			get {
				return !this.Loaded
					? new List<ImageItemController>()
					: new List<ImageItemController>(Enumerable.Range(0, this.Animation.Image.Count).Select(i => new ImageItemController() { Host = this, Index = i }));
			}
		}

		public async void uImageList_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs e
		) {
			Debug.Assert(this.Loaded);
			foreach (var item in e.AddedItems) {
				var itemX = (ImageItemController)item;
				this.ImageFilter[itemX.Index] = true;
			}
			foreach (var item in e.RemovedItems) {
				var itemX = (ImageItemController)item;
				this.ImageFilter[itemX.Index] = false;
			}
			if ((e.AddedItems.Count != 0 || e.RemovedItems.Count != 0) && this.Working) {
				var spriteIndex = this.WorkingSpriteIndex;
				await this.UnloadWorkingSprite();
				await this.LoadWorkingSprite(spriteIndex.Value);
			}
		}

		// ----------------

		public Boolean uSpriteList_IsEnabled {
			get {
				return this.Loaded;
			}
		}

		public List<SpriteItemController> uSpriteList_ItemsSource {
			get {
				return !this.Loaded
					? new List<SpriteItemController>()
					: new List<SpriteItemController>(Enumerable.Range(0, this.Animation.Sprite.Count).Select(i => new SpriteItemController() { Host = this, Index = i }));
			}
		}

		public async void uSpriteList_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs e
		) {
			Debug.Assert(this.Loaded);
			foreach (var item in e.AddedItems) {
				var itemX = (SpriteItemController)item;
				this.SpriteFilter[itemX.Index] = true;
			}
			foreach (var item in e.RemovedItems) {
				var itemX = (SpriteItemController)item;
				this.SpriteFilter[itemX.Index] = false;
			}
			if ((e.AddedItems.Count != 0 || e.RemovedItems.Count != 0) && this.Working) {
				var spriteIndex = this.WorkingSpriteIndex;
				await this.UnloadWorkingSprite();
				await this.LoadWorkingSprite(spriteIndex.Value);
			}
		}

		// ----------------

		public Boolean uMainSpriteList_IsEnabled {
			get {
				return this.Loaded && this.Animation.MainSprite != null;
			}
		}

		public List<MainSpriteItemController> uMainSpriteList_ItemsSource {
			get {
				return !this.Loaded
					? new List<MainSpriteItemController>() {
						new () {
							Host = this,
							Index = null,
						},
					}
					: new List<MainSpriteItemController>() {
						new () {
							Host = this,
							Index = this.Animation.MainSprite == null ? null : this.Animation.Sprite.Count,
						},
					};
			}
		}

		public async void uMainSpriteList_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs e
		) {
			Debug.Assert(this.Loaded);
		}

		#endregion

		#region working frame rate

		public Floating uWorkingSpriteFrameRateIcon_Opacity {
			get {
				return ConvertHelper.BooleanToFloatingOfOpacity(this.Working);
			}
		}

		// ----------------

		public Boolean uWorkingSpriteFrameRate_IsEnabled {
			get {
				return this.Working;
			}
		}

		public DecimalFormatter uWorkingSpriteFrameRate_NumberFormatter {
			get {
				return new DecimalFormatter() {
					IntegerDigits = 1,
					FractionDigits = 1,
					NumberRounder = new IncrementNumberRounder {
						Increment = 0.1,
						RoundingAlgorithm = RoundingAlgorithm.RoundHalfUp,
					},
				};
			}
		}

		public Floating uWorkingSpriteFrameRate_Value {
			get {
				return !this.Working
					? Floating.NaN
					: this.WorkingSpriteFrameRate.Value;
			}
		}

		public async void uWorkingSpriteFrameRate_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			Debug.Assert(this.Loaded);
			if (Floating.IsNaN(args.NewValue)) {
				if (this.Working) {
					this.NotifyPropertyChanged(
						nameof(this.uWorkingSpriteFrameRate_Value)
					);
				}
			} else {
				Debug.Assert(this.Working);
				var newValue = args.NewValue;
				if (newValue != this.WorkingSpriteFrameRate) {
					this.View.uSprite.Speed = newValue;
					this.WorkingSpriteFrameRate = newValue;
				}
			}
		}

		#endregion

		#region working frame range

		public Floating uWorkingSpriteFrameRangeIcon_Opacity {
			get {
				return ConvertHelper.BooleanToFloatingOfOpacity(this.Working);
			}
		}

		// ----------------

		public Boolean uWorkingSpriteFrameRangeBegin_IsEnabled {
			get {
				return this.Working;
			}
		}

		public Floating uWorkingSpriteFrameRangeBegin_Minimum {
			get {
				return !this.Working
					? Floating.NaN
					: 1.0;
			}
		}

		public Floating uWorkingSpriteFrameRangeBegin_Maximum {
			get {
				return !this.Working
					? Floating.NaN
					: AnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.Value).Frame.Count;
			}
		}

		public Floating uWorkingSpriteFrameRangeBegin_Value {
			get {
				return !this.Working
					? Floating.NaN
					: this.WorkingSpriteFrameRange.Start + 1;
			}
		}

		public async void uWorkingSpriteFrameRangeBegin_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			Debug.Assert(this.Loaded);
			if (Floating.IsNaN(args.NewValue)) {
				if (this.Working) {
					this.NotifyPropertyChanged(
						nameof(this.uWorkingSpriteFrameRangeBegin_Value)
					);
				}
			} else {
				Debug.Assert(this.Working);
				var newBegin = (Size)args.NewValue - 1;
				var newRange = new AnimationHelper.FrameRange() {
					Start = newBegin,
					Duration = this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration - newBegin,
				};
				if (newRange.Start >= this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration) {
					newRange.Duration = 1;
				}
				if (newRange != this.WorkingSpriteFrameRange) {
					await this.UpdateWorkingSpriteFrameRange(newRange);
				}
			}
		}

		// ----------------

		public Boolean uWorkingSpriteFrameRangeEnd_IsEnabled {
			get {
				return this.Working;
			}
		}

		public Floating uWorkingSpriteFrameRangeEnd_Minimum {
			get {
				return !this.Working
					? Floating.NaN
					: 1.0;
			}
		}

		public Floating uWorkingSpriteFrameRangeEnd_Maximum {
			get {
				return !this.Working
					? Floating.NaN
					: AnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.Value).Frame.Count;
			}
		}

		public Floating uWorkingSpriteFrameRangeEnd_Value {
			get {
				return !this.Working
					? Floating.NaN
					: this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration;
			}
		}

		public async void uWorkingSpriteFrameRangeEnd_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			Debug.Assert(this.Loaded);
			if (Floating.IsNaN(args.NewValue)) {
				if (this.Working) {
					this.NotifyPropertyChanged(
						nameof(this.uWorkingSpriteFrameRangeEnd_Value)
					);
				}
			} else {
				Debug.Assert(this.Working);
				var newEnd = (Size)args.NewValue - 1;
				var newRange = new AnimationHelper.FrameRange() {
					Start = this.WorkingSpriteFrameRange.Start,
					Duration = newEnd - this.WorkingSpriteFrameRange.Start + 1,
				};
				if (newRange.Duration < 1) {
					newRange.Start = newEnd;
					newRange.Duration = 1;
				}
				if (newRange != this.WorkingSpriteFrameRange) {
					await this.UpdateWorkingSpriteFrameRange(newRange);
				}
			}
		}

		#endregion

		#region working frame range label

		public Floating uWorkingSpriteFrameRangeLabelIcon_Opacity {
			get {
				return ConvertHelper.BooleanToFloatingOfOpacity(this.Working);
			}
		}

		// ----------------

		public String uWorkingSpriteFrameRangeLabel__ItemNameOfAll {
			get {
				return "...";
			}
		}

		public Boolean uWorkingSpriteFrameRangeLabel_IsEnabled {
			get {
				return this.Working;
			}
		}

		public List<String> uWorkingSpriteFrameRangeLabel_ItemsSource {
			get {
				if (!this.Working) {
					return new List<String>();
				}
				var result = (from item in this.WorkingSpriteFrameRangeLabelInformation select item.Item1).ToList();
				result.Add(this.uWorkingSpriteFrameRangeLabel__ItemNameOfAll);
				return result;
			}
		}

		public String? uWorkingSpriteFrameRangeLabel_SelectedValue {
			get {
				if (!this.Working) {
					return null;
				}
				var result = this.WorkingSpriteFrameRangeLabelInformation.Find(value => value.Item2 == this.WorkingSpriteFrameRange)?.Item1;
				return result ?? (this.WorkingSpriteFrameRange.Start == 0 && this.WorkingSpriteFrameRange.Duration == AnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.Value).Frame.Count ? this.uWorkingSpriteFrameRangeLabel__ItemNameOfAll : null);
			}
		}

		public async void uWorkingSpriteFrameRangeLabel_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs e
		) {
			Debug.Assert(this.Loaded);
			if (e.AddedItems.Count == 1) {
				Debug.Assert(this.Working);
				var newLabel = (String)e.AddedItems[0];
				var newRange = new AnimationHelper.FrameRange() {
					Start = 0,
					Duration = AnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.Value).Frame.Count,
				};
				if (newLabel != this.uWorkingSpriteFrameRangeLabel__ItemNameOfAll) {
					newRange = this.WorkingSpriteFrameRangeLabelInformation.Find(value => value.Item1 == newLabel)!.Item2;
				}
				if (newRange != this.WorkingSpriteFrameRange) {
					await this.UpdateWorkingSpriteFrameRange(newRange);
				}
			}
		}

		#endregion

		#region working frame progress

		public Boolean uWorkingSpriteFrameProgress_IsEnabled {
			get {
				return this.Working;
			}
		}

		public Floating uWorkingSpriteFrameProgress_Minimum {
			get {
				return !this.Working
					? 0.0
					: this.WorkingSpriteFrameRange.Start + 1;
			}
		}

		public Floating uWorkingSpriteFrameProgress_Maximum {
			get {
				return !this.Working
					? 0.0
					: this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration;
			}
		}

		public async void uWorkingSpriteFrameProgress_OnValueChanged (
			Object                         sender,
			RangeBaseValueChangedEventArgs e
		) {
			if (e.NewValue != 0.0) {
				Debug.Assert(this.Loaded && this.Working);
				Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
				if (this.uWorkingSpriteFrameProgress__Changeable) {
					this.View.uSprite.CurrentTime = TimeSpan.FromSeconds(e.NewValue - 1.0);
				}
			}
		}

		// ----------------

		public Boolean uWorkingSpriteFrameProgress__Changeable = false;

		public Boolean uWorkingSpriteFrameProgress__ChangingWhenPlaying = false;

		public async void uWorkingSpriteFrameProgress_OnPointerPressed (
			Object                 sender,
			PointerRoutedEventArgs e
		) {
			Debug.Assert(this.Loaded && this.Working);
			Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
			this.uWorkingSpriteFrameProgress__Changeable = true;
			this.uWorkingSpriteFrameProgress__ChangingWhenPlaying = !this.WorkingSpritePaused.Value;
			if (this.uWorkingSpriteFrameProgress__ChangingWhenPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
				this.WorkingSpritePaused = true;
				this.NotifyPropertyChanged(
					nameof(this.uWorkingSpritePauseIcon_Glyph)
				);
			}
		}

		public async void uWorkingSpriteFrameProgress_OnPointerReleased (
			Object                 sender,
			PointerRoutedEventArgs e
		) {
			Debug.Assert(this.Loaded && this.Working);
			Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
			if (this.uWorkingSpriteFrameProgress__ChangingWhenPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Playing;
				this.WorkingSpritePaused = false;
				this.NotifyPropertyChanged(
					nameof(this.uWorkingSpritePauseIcon_Glyph)
				);
			}
			this.uWorkingSpriteFrameProgress__Changeable = false;
			this.uWorkingSpriteFrameProgress__ChangingWhenPlaying = false;
		}

		#endregion

		#region working frame progress control

		public Boolean uWorkingSpritePause_IsEnabled {
			get {
				return this.Working;
			}
		}

		public async void uWorkingSpritePause_OnClick (
			Object          sender,
			RoutedEventArgs e
		) {
			Debug.Assert(this.Loaded && this.Working);
			Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
			var newPaused = this.View.uSprite.State == SpriteControl.StateType.Playing;
			this.View.uSprite.State = newPaused ? SpriteControl.StateType.Paused : SpriteControl.StateType.Playing;
			this.WorkingSpritePaused = newPaused;
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpritePauseIcon_Glyph)
			);
		}

		// ----------------

		public String uWorkingSpritePauseIcon_Glyph {
			get {
				return !this.Working
					? FluentIconGlyph.Pause
					: this.WorkingSpritePaused.Value
						? FluentIconGlyph.Play
						: FluentIconGlyph.Pause;
			}
		}

		// ----------------

		public Boolean uWorkingSpritePrev_IsEnabled {
			get {
				return this.Working;
			}
		}

		public async void uWorkingSpritePrev_OnClick (
			Object          sender,
			RoutedEventArgs e
		) {
			Debug.Assert(this.Loaded && this.Working);
			Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
			var newTime = this.View.uSprite.CurrentTime - TimeSpan.FromSeconds(1.0);
			var beginTime = TimeSpan.FromSeconds(this.WorkingSpriteFrameRange.Start);
			if (newTime < beginTime) {
				newTime = beginTime;
			}
			this.View.uSprite.CurrentTime = newTime;
		}

		// ----------------

		public Boolean uWorkingSpriteNext_IsEnabled {
			get {
				return this.Working;
			}
		}

		public async void uWorkingSpriteNext_OnClick (
			Object          sender,
			RoutedEventArgs e
		) {
			Debug.Assert(this.Loaded && this.Working);
			Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
			var newTime = this.View.uSprite.CurrentTime + TimeSpan.FromSeconds(1.0);
			var endTime = TimeSpan.FromSeconds(this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration - 1.0);
			if (newTime > endTime) {
				newTime = endTime;
			}
			this.View.uSprite.CurrentTime = newTime;
		}

		#endregion

		#region show sprite boundary

		public Boolean uShowSpriteBoundary_IsChecked {
			get {
				return this.ShowSpriteBoundary;
			}
		}

		public async void uShowSpriteBoundary_OnChecked (
			Object          sender,
			RoutedEventArgs e
		) {
			var senderX = (ToggleButton)e.OriginalSource;
			var newValue = senderX.IsChecked!.Value;
			this.ShowSpriteBoundary = newValue;
			this.View.uSprite.ShowBoundary = this.ShowSpriteBoundary;
			this.NotifyPropertyChanged(
				nameof(this.uShowSpriteBoundary_IsChecked)
			);
		}

		#endregion

		#region plant custom layer

		public Floating uPlantCustomLayerIcon_Opacity {
			get {
				return ConvertHelper.BooleanToFloatingOfOpacity(this.Loaded);
			}
		}

		// ----------------

		public Boolean uPlantCustomLayer_IsEnabled {
			get {
				return this.Loaded;
			}
		}

		public List<String> uPlantCustomLayer_ItemsSource {
			get {
				if (!this.Loaded) {
					return new List<String>();
				}
				var result = new List<String>(from item in this.PlantCustomLayerName select item["custom_".Length..]);
				result.Insert(0, "\0");
				return result;
			}
		}

		public String? uPlantCustomLayer_SelectedValue {
			get {
				if (!this.Loaded) {
					return null;
				}
				var selectedItem = (from item in this.PlantCustomLayerName where this.View.uSpriteList.SelectedItems.Contains(this.View.uSpriteList.Items[this.Animation.Sprite.FindIndex(value => value.Name == item)]) select item).ToList();
				return selectedItem.Count == 1 ? selectedItem[0] : selectedItem.Count == 0 ? "\0" : null;
			}
		}

		public async void uPlantCustomLayer_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs e
		) {
			// TODO : avoid repeat rendering
			if (this.Loaded && e.AddedItems.Count == 1) {
				var targetCustomName = $"custom_{(String)e.AddedItems[0]}";
				foreach (var customName in this.PlantCustomLayerName) {
					for (var index = 0; index < this.Animation.Sprite.Count; ++index) {
						var sprite = this.Animation.Sprite[index];
						if (sprite.Name == customName) {
							if (sprite.Name == targetCustomName) {
								this.View.uSpriteList.SelectRange(new ItemIndexRange(index, 1));
							} else {
								this.View.uSpriteList.DeselectRange(new ItemIndexRange(index, 1));
							}
						}
					}
				}
				this.NotifyPropertyChanged(
					nameof(this.uPlantCustomLayer_SelectedValue)
				);
			}
		}

		#endregion

		#region zombie state layer

		public Floating uZombieStateLayerIcon_Opacity {
			get {
				return ConvertHelper.BooleanToFloatingOfOpacity(this.Loaded);
			}
		}

		// ----------------

		public Boolean uZombieStateLayer_IsEnabled {
			get {
				return this.Loaded;
			}
		}

		public List<String> uZombieStateLayer_ItemsSource {
			get {
				if (!this.Loaded) {
					return new List<String>();
				}
				var result = new List<String>(this.ZombieStateLayerName);
				result.Insert(0, "\0");
				return result;
			}
		}

		public String? uZombieStateLayer_SelectedValue {
			get {
				if (!this.Loaded) {
					return null;
				}
				var selectedItem = (from item in this.ZombieStateLayerName where this.View.uSpriteList.SelectedItems.Contains(this.View.uSpriteList.Items[this.Animation.Sprite.FindIndex(value => value.Name == item)]) select item).ToList();
				return selectedItem.Count == 1 ? selectedItem[0] : selectedItem.Count == 0 ? "\0" : null;
			}
		}

		public async void uZombieStateLayer_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs e
		) {
			// TODO : avoid repeat rendering
			if (this.Loaded && e.AddedItems.Count == 1) {
				var targetCustomName = (String)e.AddedItems[0];
				foreach (var customName in this.ZombieStateLayerName) {
					for (var index = 0; index < this.Animation.Sprite.Count; ++index) {
						var sprite = this.Animation.Sprite[index];
						if (sprite.Name == customName) {
							if (sprite.Name == targetCustomName) {
								this.View.uSpriteList.SelectRange(new ItemIndexRange(index, 1));
							} else {
								this.View.uSpriteList.DeselectRange(new ItemIndexRange(index, 1));
							}
						}
					}
				}
				this.NotifyPropertyChanged(
					nameof(this.uZombieStateLayer_SelectedValue)
				);
			}
		}

		#endregion

		#region zombie ground swatch layer

		public Floating uZombieGroundSwatchLayerIcon_Opacity {
			get {
				return ConvertHelper.BooleanToFloatingOfOpacity(this.Loaded);
			}
		}

		// ----------------

		public Boolean uZombieGroundSwatchLayer_IsEnabled {
			get {
				return this.Loaded;
			}
		}

		public Boolean uZombieGroundSwatchLayer_IsChecked {
			get {
				if (!this.Loaded) {
					return false;
				}
				var selectedItem = (from item in this.ZombieGroundSwatchLayerName where this.View.uSpriteList.SelectedItems.Contains(this.View.uSpriteList.Items[this.Animation.Sprite.FindIndex(value => value.Name == item)]) select item).ToList();
				return selectedItem.Count == 2;
			}
		}

		public async void uZombieGroundSwatchLayer_OnChecked (
			Object          sender,
			RoutedEventArgs e
		) {
			var senderX = (ToggleButton)e.OriginalSource;
			// TODO : avoid repeat rendering
			if (this.Loaded) {
				var newValue = senderX.IsChecked!.Value;
				for (var index = 0; index < this.Animation.Sprite.Count; ++index) {
					var sprite = this.Animation.Sprite[index];
					if (this.ZombieGroundSwatchLayerName.Contains(sprite.Name)) {
						if (newValue) {
							this.View.uSpriteList.SelectRange(new ItemIndexRange(index, 1));
						} else {
							this.View.uSpriteList.DeselectRange(new ItemIndexRange(index, 1));
						}
					}
				}
				this.NotifyPropertyChanged(
					nameof(this.uZombieGroundSwatchLayer_IsChecked)
				);
			}
		}

		#endregion

	}

	public class ImageItemController : CustomController {

		#region data

		public required MainPageController Host;

		public required Size Index;

		#endregion

		#region view

		public String uTitle_Text {
			get {
				Debug.Assert(this.Host.Loaded);
				var model = this.Host.Animation.Image[this.Index];
				return $"{model.Name.Split('|')[0]}";
			}
		}

		// ----------------

		public String uDescription_Text {
			get {
				Debug.Assert(this.Host.Loaded);
				var model = this.Host.Animation.Image[this.Index];
				return $"{model.Size[0]} x {model.Size[1]}";
			}
		}

		// ----------------

		public UIElement uPreview_Content {
			get {
				Debug.Assert(this.Host.Loaded);
				var source = this.Host.ImageSource[this.Index];
				if (source != null) {
					return new Image() {
						Width = 32, Height = 32,
						Source = source,
					};
				} else {
					return new FontIcon() {
						Width = 32, Height = 32,
						FontSize = 20,
						Opacity = 0.7,
						Glyph = FluentIconGlyph.Error,
					};
				}
			}
		}

		#endregion

	}

	public class SpriteItemController : CustomController {

		#region data

		public required MainPageController Host;

		public required Size Index;

		#endregion

		#region view

		public String uTitle_Text {
			get {
				Debug.Assert(this.Host.Loaded);
				var model = this.Host.Animation.Sprite[this.Index];
				return $"{model.Name}";
			}
		}

		// ----------------

		public String uDescription_Text {
			get {
				Debug.Assert(this.Host.Loaded);
				var model = this.Host.Animation.Sprite[this.Index];
				return $"{model.FrameRate:F0} - {model.Frame.Count}";
			}
		}

		// ----------------

		public UIElement uPreview_Content {
			get {
				Debug.Assert(this.Host.Loaded);
				var model = this.Host.Animation.Sprite[this.Index];
				var source = (ImageSource?)null;
				if (model.Frame.Count == 1 && model.Frame[0].Append.Count == 1 && model.Frame[0].Change.Count == 1 && !model.Frame[0].Append[0].Sprite) {
					source = this.Host.ImageSource[(Size)model.Frame[0].Append[0].Resource];
				}
				if (source != null) {
					return new Image() {
						Width = 32, Height = 32,
						Source = source,
					};
				} else {
					return new FontIcon() {
						Width = 32, Height = 32,
						FontSize = 20,
						Opacity = 0.7,
						Glyph = FluentIconGlyph.IOT,
					};
				}
			}
		}

		// ----------------

		public Boolean uToggle_IsChecked {
			get {
				Debug.Assert(this.Host.Loaded);
				return this.Index == this.Host.WorkingSpriteIndex;
			}
		}

		public async void uToggle_OnClick (
			Object          sender,
			RoutedEventArgs e
		) {
			Debug.Assert(this.Host.Loaded);
			var lastWorkingSpriteIndex = this.Host.WorkingSpriteIndex;
			if (this.Host.Working) {
				await this.Host.UnloadWorkingSprite();
			}
			if (this.Index != lastWorkingSpriteIndex) {
				await this.Host.LoadWorkingSprite(this.Index);
			}
		}

		#endregion

	}

	public class MainSpriteItemController : CustomController {

		#region data

		public required MainPageController Host;

		public required Size? Index;

		#endregion

		#region view

		public Floating uRoot_Opacity {
			get {
				if (this.Index == null) {
					return 0.0;
				} else {
					Debug.Assert(this.Host.Loaded);
					return 1.0;
				}
			}
		}

		// ----------------

		public String uTitle_Text {
			get {
				if (this.Index == null) {
					return "";
				} else {
					Debug.Assert(this.Host.Loaded);
					var model = this.Host.Animation.MainSprite!;
					return $"{model.Name}";
				}
			}
		}

		// ----------------

		public String uDescription_Text {
			get {
				if (this.Index == null) {
					return "";
				} else {
					Debug.Assert(this.Host.Loaded);
					var model = this.Host.Animation.MainSprite!;
					return $"{model.FrameRate:F0} - {model.Frame.Count}";
				}
			}
		}

		// ----------------

		public UIElement uPreview_Content {
			get {
				return new FontIcon() {
					Width = 32, Height = 32,
					FontSize = 20,
					Opacity = 0.7,
					Glyph = FluentIconGlyph.IOT,
				};
			}
		}

		// ----------------

		public Boolean uToggle_IsChecked {
			get {
				if (this.Index == null) {
					return false;
				} else {
					Debug.Assert(this.Host.Loaded);
					return this.Index == this.Host.WorkingSpriteIndex;
				}
			}
		}

		public async void uToggle_OnClick (
			Object          sender,
			RoutedEventArgs e
		) {
			if (this.Index == null) {
			} else {
				Debug.Assert(this.Host.Loaded);
				var lastWorkingSpriteIndex = this.Host.WorkingSpriteIndex;
				if (this.Host.Working) {
					await this.Host.UnloadWorkingSprite();
				}
				if (this.Index != lastWorkingSpriteIndex) {
					await this.Host.LoadWorkingSprite(this.Index.Value);
				}
			}
		}

		#endregion

	}

}
