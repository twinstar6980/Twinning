import '/common.dart';
import '/utility/system_overlay_helper.dart';
import '/widget/common.dart';
import '/widget/container.dart';
import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter/material.dart' as material;
import 'package:dynamic_color/dynamic_color.dart' as lib;

// ----------------

// #region color

enum StyledColor {
  disabled,
  primary,
  primaryContainer,
  onPrimary,
  secondary,
  secondaryContainer,
  onSecondary,
  tertiary,
  tertiaryContainer,
  onTertiary,
  error,
  errorContainer,
  onError,
  surface,
  surfaceTint,
  surfaceContainerLowest,
  surfaceContainerLow,
  surfaceContainer,
  surfaceContainerHigh,
  surfaceContainerHighest,
  onSurface,
  onSurfaceVariant,
}

extension StyledColorExtension<TType extends Widget> on TType {

  static Color value(
    BuildContext context,
    StyledColor  variant,
  ) {
    return switch (variant) {
      StyledColor.disabled                => material.Theme.of(context).disabledColor,
      StyledColor.primary                 => material.Theme.of(context).colorScheme.primary,
      StyledColor.primaryContainer        => material.Theme.of(context).colorScheme.primaryContainer,
      StyledColor.onPrimary               => material.Theme.of(context).colorScheme.onPrimary,
      StyledColor.secondary               => material.Theme.of(context).colorScheme.secondary,
      StyledColor.secondaryContainer      => material.Theme.of(context).colorScheme.secondaryContainer,
      StyledColor.onSecondary             => material.Theme.of(context).colorScheme.onSecondary,
      StyledColor.tertiary                => material.Theme.of(context).colorScheme.tertiary,
      StyledColor.tertiaryContainer       => material.Theme.of(context).colorScheme.tertiaryContainer,
      StyledColor.onTertiary              => material.Theme.of(context).colorScheme.onTertiary,
      StyledColor.error                   => material.Theme.of(context).colorScheme.error,
      StyledColor.errorContainer          => material.Theme.of(context).colorScheme.errorContainer,
      StyledColor.onError                 => material.Theme.of(context).colorScheme.onError,
      StyledColor.surface                 => material.Theme.of(context).colorScheme.surface,
      StyledColor.surfaceTint             => material.Theme.of(context).colorScheme.surfaceTint,
      StyledColor.surfaceContainerLowest  => material.Theme.of(context).colorScheme.surfaceContainerLowest,
      StyledColor.surfaceContainerLow     => material.Theme.of(context).colorScheme.surfaceContainerLow,
      StyledColor.surfaceContainer        => material.Theme.of(context).colorScheme.surfaceContainer,
      StyledColor.surfaceContainerHigh    => material.Theme.of(context).colorScheme.surfaceContainerHigh,
      StyledColor.surfaceContainerHighest => material.Theme.of(context).colorScheme.surfaceContainerHighest,
      StyledColor.onSurface               => material.Theme.of(context).colorScheme.onSurface,
      StyledColor.onSurfaceVariant        => material.Theme.of(context).colorScheme.onSurfaceVariant,
    };
  }

}

// #endregion

// #region material

enum StyledMaterialVariant {
  standard,
}

class StyledMaterial extends StatelessWidget {

  const StyledMaterial._({
    super.key,
    required this.variant,
    required this.child,
  });

  const StyledMaterial.standard({
    Key?            key = null,
    required Widget child,
  }) : this._(
    key: key,
    variant: StyledMaterialVariant.standard,
    child: child,
  );

  // ----------------

  final StyledMaterialVariant variant;
  final Widget                child;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledMaterialVariant.standard => material.Material(
        type: material.MaterialType.transparency,
        child: this.child,
      ),
    };
  }

}

extension StyledMaterialExtension<TType extends Widget> on TType {

  StyledMaterial withStyledMaterial(
  ) {
    return StyledMaterial.standard(
      child: this,
    );
  }

}

// #endregion

// #region message

enum StyledTooltipVariant {
  standard,
}

class StyledTooltip extends StatelessWidget {

  const StyledTooltip._({
    super.key,
    required this.variant,
    required this.message,
    required this.child,
  });

  const StyledTooltip.standard({
    Key?            key = null,
    String?         message = null,
    required Widget child,
  }) : this._(
    key: key,
    variant: StyledTooltipVariant.standard,
    message: message,
    child: child,
  );

  // ----------------

  final StyledTooltipVariant variant;
  final String?              message;
  final Widget               child;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledTooltipVariant.standard => material.Tooltip(
        message: this.message == null ? null : this.message == '' ? ' ' : this.message,
        child: this.child,
      ),
    };
  }

}

extension StyledTooltipExtension<TType extends Widget> on TType {

  StyledTooltip withStyledTooltip({
    required String? message,
  }) {
    return StyledTooltip.standard(
      message: message,
      child: this,
    );
  }

}

// ----------------

enum StyledBadgeVariant {
  standard,
}

class StyledBadge extends StatelessWidget {

  const StyledBadge._({
    super.key,
    required this.variant,
    required this.visible,
    required this.label,
    required this.child,
  });

  const StyledBadge.standard({
    Key?    key = null,
    Boolean visible = true,
    Widget? label = null,
    Widget? child = null,
  }) : this._(
    key: key,
    variant: StyledBadgeVariant.standard,
    visible: visible,
    label: label,
    child: child,
  );

  // ----------------

  final StyledBadgeVariant variant;
  final Boolean            visible;
  final Widget?            label;
  final Widget?            child;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledBadgeVariant.standard => material.Badge(
        isLabelVisible: this.visible,
        label: this.label,
        child: this.child,
      ),
    };
  }

}

// ----------------

enum StyledDividerVariant {
  standard,
  minimal,
}

class StyledDivider extends StatelessWidget {

  const StyledDivider._({
    super.key,
    required this.variant,
    required this.indent,
  });

  const StyledDivider.standard({
    Key?    key = null,
    Floater indent = 0,
  }) : this._(
    key: key,
    variant: StyledDividerVariant.standard,
    indent: indent,
  );

  const StyledDivider.minimal({
    Key?    key = null,
    Floater indent = 0,
  }) : this._(
    key: key,
    variant: StyledDividerVariant.minimal,
    indent: indent,
  );

  // ----------------

  final StyledDividerVariant variant;
  final Floater              indent;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledDividerVariant.standard => material.Divider(
        height: 16,
        indent: this.indent,
        endIndent: this.indent,
      ),
      StyledDividerVariant.minimal => material.Divider(
        height: 1,
        indent: this.indent,
        endIndent: this.indent,
      ),
    };
  }

}

// ----------------

enum StyledProgressVariant {
  linear,
  circular,
}

class StyledProgress extends StatelessWidget {

  const StyledProgress._({
    super.key,
    required this.variant,
    required this.paused,
    required this.value,
  });

  const StyledProgress.linear({
    Key?     key = null,
    Boolean  paused = false,
    Floater? value = null,
  }) : this._(
    key: key,
    variant: StyledProgressVariant.linear,
    paused: paused,
    value: value,
  );

  const StyledProgress.circular({
    Key?     key = null,
    Boolean  paused = false,
    Floater? value = null,
  }) : this._(
    key: key,
    variant: StyledProgressVariant.circular,
    paused: paused,
    value: value,
  );

  // ----------------

  final StyledProgressVariant variant;
  final Boolean               paused;
  final Floater?              value;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledProgressVariant.linear => material.LinearProgressIndicator(
        color: !this.paused ? null : material.Theme.of(context).colorScheme.tertiary,
        value: this.value ?? (!this.paused ? null : 1),
      ),
      StyledProgressVariant.circular => material.CircularProgressIndicator(
        color: !this.paused ? null : material.Theme.of(context).colorScheme.tertiary,
        value: this.value ?? (!this.paused ? null : 1),
      ),
    };
  }

}

// ----------------

extension StyledSnackExtension<TType extends Widget> on TType {

  static Future<Void> show(
    BuildContext context,
    String       content,
  ) async {
    material.ScaffoldMessenger.of(context).showSnackBar(material.SnackBar(
      behavior: material.SnackBarBehavior.floating,
      content: StyledText.custom(
        content,
        overflow: TextOverflow.clip,
      ),
    ));
    return;
  }

}

// #endregion

// #region text

enum StyledTextVariant {
  displayLarge,
  displayMedium,
  displaySmall,
  headlineLarge,
  headlineMedium,
  headlineSmall,
  titleLarge,
  titleMedium,
  titleSmall,
  bodyLarge,
  bodyMedium,
  bodySmall,
  labelLarge,
  labelMedium,
  labelSmall,
}

typedef StyledTextOverflow = TextOverflow;

typedef StyledTextAlign = TextAlign;

// TODO: unfinished
class StyledText extends StatelessWidget {

  const StyledText._({
    super.key,
    required this.text,
    required this.variant,
    required this.tooltip,
    required this.tooltipText,
    required this.color,
    required this.overflow,
    required this.align,
    required this.style,
  });

  const StyledText.inherit(
    String             text, {
    Key?               key = null,
    Boolean            tooltip = false,
    String?            tooltipText = null,
    StyledColor?       color = null,
    StyledTextOverflow overflow = TextOverflow.ellipsis,
    StyledTextAlign    align = TextAlign.start,
    TextStyle?         style = null,
  }) : this._(
    key: key,
    text: text,
    variant: null,
    tooltip: tooltip,
    tooltipText: tooltipText,
    color: color,
    overflow: overflow,
    align: align,
    style: style,
  );

  const StyledText.variant(
    StyledTextVariant? variant,
    String             text, {
    Key?               key = null,
    Boolean            tooltip = false,
    String?            tooltipText = null,
    StyledColor?       color = null,
    StyledTextOverflow overflow = TextOverflow.ellipsis,
    StyledTextAlign    align = TextAlign.start,
    TextStyle?         style = null,
  }) : this._(
    key: key,
    text: text,
    variant: variant,
    tooltip: tooltip,
    tooltipText: tooltipText,
    color: color,
    overflow: overflow,
    align: align,
    style: style,
  );

  const StyledText.custom(
    String             text, {
    Key?               key = null,
    StyledTextVariant? variant = null,
    Boolean            tooltip = false,
    String?            tooltipText = null,
    StyledColor?       color = null,
    StyledTextOverflow overflow = TextOverflow.ellipsis,
    StyledTextAlign    align = TextAlign.start,
    TextStyle?         style = null,
  }) : this._(
    key: key,
    text: text,
    variant: variant,
    tooltip: tooltip,
    tooltipText: tooltipText,
    color: color,
    overflow: overflow,
    align: align,
    style: style,
  );

  // ----------------

  final String             text;
  final StyledTextVariant? variant;
  final StyledColor?       color;
  final Boolean            tooltip;
  final String?            tooltipText;
  final StyledTextOverflow overflow;
  final StyledTextAlign    align;
  final TextStyle?         style;

  // ----------------

  @override
  build(context) {
    var style = switch (this.variant) {
      null                             => TextStyle(inherit: true),
      StyledTextVariant.displayLarge   => material.Theme.of(context).textTheme.displayLarge!,
      StyledTextVariant.displayMedium  => material.Theme.of(context).textTheme.displayMedium!,
      StyledTextVariant.displaySmall   => material.Theme.of(context).textTheme.displaySmall!,
      StyledTextVariant.headlineLarge  => material.Theme.of(context).textTheme.headlineLarge!,
      StyledTextVariant.headlineMedium => material.Theme.of(context).textTheme.headlineMedium!,
      StyledTextVariant.headlineSmall  => material.Theme.of(context).textTheme.headlineSmall!,
      StyledTextVariant.titleLarge     => material.Theme.of(context).textTheme.titleLarge!,
      StyledTextVariant.titleMedium    => material.Theme.of(context).textTheme.titleMedium!,
      StyledTextVariant.titleSmall     => material.Theme.of(context).textTheme.titleSmall!,
      StyledTextVariant.bodyLarge      => material.Theme.of(context).textTheme.bodyLarge!,
      StyledTextVariant.bodyMedium     => material.Theme.of(context).textTheme.bodyMedium!,
      StyledTextVariant.bodySmall      => material.Theme.of(context).textTheme.bodySmall!,
      StyledTextVariant.labelLarge     => material.Theme.of(context).textTheme.labelLarge!,
      StyledTextVariant.labelMedium    => material.Theme.of(context).textTheme.labelMedium!,
      StyledTextVariant.labelSmall     => material.Theme.of(context).textTheme.labelSmall!,
    }.merge(this.style).copyWith(
      color: this.color == null ? null : StyledColorExtension.value(context, this.color!),
    );
    return Text(
      this.text,
      overflow: this.overflow,
      textAlign: this.align,
      style: style,
    ).selfLet((it) => !this.tooltip ? it : it.withStyledTooltip(message: this.tooltipText ?? this.text));
  }

}

// #endregion

// #region button

enum StyledButtonVariant {
  text,
  filled,
  filledTonal,
  outlined,
}

typedef StyledButtonIconAlign = material.IconAlignment;

class StyledButton extends StatelessWidget {

  const StyledButton._({
    super.key,
    required this.variant,
    required this.enabled,
    required this.tooltip,
    required this.iconAlign,
    required this.icon,
    required this.content,
    required this.onPressed,
  });

  const StyledButton.text({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    String?                                      tooltip = null,
    StyledButtonIconAlign                        iconAlign = StyledButtonIconAlign.start,
    Widget?                                      icon = null,
    required Widget                              content,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledButtonVariant.text,
    enabled: enabled,
    tooltip: tooltip,
    iconAlign: iconAlign,
    icon: icon,
    content: content,
    onPressed: onPressed,
  );

  const StyledButton.filled({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    String?                                      tooltip = null,
    StyledButtonIconAlign                        iconAlign = StyledButtonIconAlign.start,
    Widget?                                      icon = null,
    required Widget                              content,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledButtonVariant.filled,
    enabled: enabled,
    tooltip: tooltip,
    iconAlign: iconAlign,
    icon: icon,
    content: content,
    onPressed: onPressed,
  );

  const StyledButton.filledTonal({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    String?                                      tooltip = null,
    StyledButtonIconAlign                        iconAlign = StyledButtonIconAlign.start,
    Widget?                                      icon = null,
    required Widget                              content,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledButtonVariant.filledTonal,
    enabled: enabled,
    tooltip: tooltip,
    iconAlign: iconAlign,
    icon: icon,
    content: content,
    onPressed: onPressed,
  );

  const StyledButton.outlined({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    String?                                      tooltip = null,
    StyledButtonIconAlign                        iconAlign = StyledButtonIconAlign.start,
    Widget?                                      icon = null,
    required Widget                              content,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledButtonVariant.outlined,
    enabled: enabled,
    tooltip: tooltip,
    iconAlign: iconAlign,
    icon: icon,
    content: content,
    onPressed: onPressed,
  );

  // ----------------

  final StyledButtonVariant                 variant;
  final Boolean                             enabled;
  final String?                             tooltip;
  final StyledButtonIconAlign               iconAlign;
  final Widget?                             icon;
  final Widget                              content;
  final Void Function(BuildContext context) onPressed;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledButtonVariant.text => material.TextButton.icon(
        iconAlignment: this.iconAlign,
        icon: this.icon,
        label: this.content,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
      StyledButtonVariant.filled => material.FilledButton.icon(
        iconAlignment: this.iconAlign,
        icon: this.icon,
        label: this.content,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
      StyledButtonVariant.filledTonal => material.FilledButton.tonalIcon(
        iconAlignment: this.iconAlign,
        icon: this.icon,
        label: this.content,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
      StyledButtonVariant.outlined => material.OutlinedButton.icon(
        iconAlignment: this.iconAlign,
        icon: this.icon,
        label: this.content,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
    }.selfLet((it) => !this.enabled || this.tooltip == null ? it : it.withStyledTooltip(message: this.tooltip!));
  }

}

// ----------------

enum StyledIconButtonVariant {
  standard,
  filled,
  filledTonal,
  outlined,
}

class StyledIconButton extends StatelessWidget {

  const StyledIconButton._({
    super.key,
    required this.variant,
    required this.enabled,
    required this.tooltip,
    required this.selected,
    required this.icon,
    required this.iconOnSelected,
    required this.onPressed,
  });

  const StyledIconButton.standard({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    String?                                      tooltip = null,
    Boolean?                                     selected,
    required Widget                              icon,
    Widget?                                      iconOnSelected,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledIconButtonVariant.standard,
    enabled: enabled,
    tooltip: tooltip,
    selected: selected,
    icon: icon,
    iconOnSelected: iconOnSelected,
    onPressed: onPressed,
  );

  const StyledIconButton.filled({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    String?                                      tooltip = null,
    Boolean?                                     selected,
    required Widget                              icon,
    Widget?                                      iconOnSelected,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledIconButtonVariant.filled,
    enabled: enabled,
    tooltip: tooltip,
    selected: selected,
    icon: icon,
    iconOnSelected: iconOnSelected,
    onPressed: onPressed,
  );

  const StyledIconButton.filledTonal({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    String?                                      tooltip = null,
    Boolean?                                     selected,
    required Widget                              icon,
    Widget?                                      iconOnSelected,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledIconButtonVariant.filledTonal,
    enabled: enabled,
    tooltip: tooltip,
    selected: selected,
    icon: icon,
    iconOnSelected: iconOnSelected,
    onPressed: onPressed,
  );

  const StyledIconButton.outlined({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    String?                                      tooltip = null,
    Boolean?                                     selected,
    required Widget                              icon,
    Widget?                                      iconOnSelected,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledIconButtonVariant.outlined,
    enabled: enabled,
    tooltip: tooltip,
    selected: selected,
    icon: icon,
    iconOnSelected: iconOnSelected,
    onPressed: onPressed,
  );

  // ----------------

  final StyledIconButtonVariant             variant;
  final Boolean                             enabled;
  final String?                             tooltip;
  final Boolean?                            selected;
  final Widget                              icon;
  final Widget?                             iconOnSelected;
  final Void Function(BuildContext context) onPressed;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledIconButtonVariant.standard => material.IconButton(
        isSelected: this.selected,
        icon: this.icon,
        selectedIcon: this.iconOnSelected,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
      StyledIconButtonVariant.filled => material.IconButton.filled(
        isSelected: this.selected,
        icon: this.icon,
        selectedIcon: this.iconOnSelected,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
      StyledIconButtonVariant.filledTonal => material.IconButton.filledTonal(
        isSelected: this.selected,
        icon: this.icon,
        selectedIcon: this.iconOnSelected,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
      StyledIconButtonVariant.outlined => material.IconButton.outlined(
        isSelected: this.selected,
        icon: this.icon,
        selectedIcon: this.iconOnSelected,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
    }.selfLet((it) => !this.enabled || this.tooltip == null ? it : it.withStyledTooltip(message: this.tooltip!));
  }

}

// ----------------

enum StyledFloatingButtonVariant {
  standard,
  smaller,
  larger,
  expanded,
}

class StyledFloatingButton extends StatelessWidget {

  const StyledFloatingButton._({
    super.key,
    required this.variant,
    required this.enabled,
    required this.tooltip,
    required this.icon,
    required this.label,
    required this.onPressed,
  });

  const StyledFloatingButton.standard({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    required String?                             tooltip,
    required Widget                              icon,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledFloatingButtonVariant.standard,
    enabled: enabled,
    tooltip: tooltip,
    icon: icon,
    label: null,
    onPressed: onPressed,
  );

  const StyledFloatingButton.smaller({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    required String?                             tooltip,
    required Widget                              icon,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledFloatingButtonVariant.smaller,
    enabled: enabled,
    tooltip: tooltip,
    icon: icon,
    label: null,
    onPressed: onPressed,
  );

  const StyledFloatingButton.larger({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    required String?                             tooltip,
    required Widget                              icon,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledFloatingButtonVariant.larger,
    enabled: enabled,
    tooltip: tooltip,
    icon: icon,
    label: null,
    onPressed: onPressed,
  );

  const StyledFloatingButton.expanded({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    required String?                             tooltip,
    required Widget                              icon,
    required Widget                              label,
    required Void Function(BuildContext context) onPressed,
  }) : this._(
    key: key,
    variant: StyledFloatingButtonVariant.expanded,
    enabled: enabled,
    tooltip: tooltip,
    icon: icon,
    label: label,
    onPressed: onPressed,
  );

  // ----------------

  final StyledFloatingButtonVariant         variant;
  final Boolean                             enabled;
  final String?                             tooltip;
  final Widget                              icon;
  final Widget?                             label;
  final Void Function(BuildContext context) onPressed;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledFloatingButtonVariant.standard => material.FloatingActionButton(
        elevation: 0,
        focusElevation: 0,
        hoverElevation: 0,
        highlightElevation: 0,
        disabledElevation: 0,
        child: this.icon,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
      StyledFloatingButtonVariant.smaller => material.FloatingActionButton.small(
        elevation: 0,
        focusElevation: 0,
        hoverElevation: 0,
        highlightElevation: 0,
        disabledElevation: 0,
        child: this.icon,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
      StyledFloatingButtonVariant.larger => material.FloatingActionButton.large(
        elevation: 0,
        focusElevation: 0,
        hoverElevation: 0,
        highlightElevation: 0,
        disabledElevation: 0,
        child: this.icon,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
      StyledFloatingButtonVariant.expanded => material.FloatingActionButton.extended(
        elevation: 0,
        focusElevation: 0,
        hoverElevation: 0,
        highlightElevation: 0,
        disabledElevation: 0,
        icon: this.icon,
        label: this.label!,
        onPressed: !this.enabled ? null : () => this.onPressed(context),
      ),
    }.selfLet((it) => !this.enabled || this.tooltip == null ? it : it.withStyledTooltip(message: this.tooltip!));
  }

}

// #endregion

// #region input

enum StyledCheckVariant {
  standard,
}

class StyledCheck extends StatelessWidget {

  const StyledCheck._({
    super.key,
    required this.variant,
    required this.enabled,
    required this.tooltip,
    required this.value,
    required this.onChanged,
  });

  const StyledCheck.standard({
    Key?                                                         key = null,
    Boolean                                                      enabled = true,
    String?                                                      tooltip = null,
    required Boolean                                             value,
    required Void Function(BuildContext context, Boolean? value) onChanged,
  }) : this._(
    key: key,
    variant: StyledCheckVariant.standard,
    enabled: enabled,
    tooltip: tooltip,
    value: value,
    onChanged: onChanged,
  );

  // ----------------

  final StyledCheckVariant                                  variant;
  final Boolean                                             enabled;
  final String?                                             tooltip;
  final Boolean                                             value;
  final Void Function(BuildContext context, Boolean? value) onChanged;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledCheckVariant.standard => material.Checkbox(
        value: this.value,
        onChanged: !this.enabled ? null : (value) => this.onChanged(context, value),
      ),
    }.selfLet((it) => !this.enabled || this.tooltip == null ? it : it.withStyledTooltip(message: this.tooltip!));
  }

}

// ----------------

enum StyledSwitchVariant {
  standard,
}

class StyledSwitch extends StatelessWidget {

  const StyledSwitch._({
    super.key,
    required this.variant,
    required this.enabled,
    required this.tooltip,
    required this.value,
    required this.onChanged,
  });

  const StyledSwitch.standard({
    Key?                                                        key = null,
    Boolean                                                     enabled = true,
    String?                                                     tooltip = null,
    required Boolean                                            value,
    required Void Function(BuildContext context, Boolean value) onChanged,
  }) : this._(
    key: key,
    variant: StyledSwitchVariant.standard,
    enabled: enabled,
    tooltip: tooltip,
    value: value,
    onChanged: onChanged,
  );

  // ----------------

  final StyledSwitchVariant                                variant;
  final Boolean                                            enabled;
  final String?                                            tooltip;
  final Boolean                                            value;
  final Void Function(BuildContext context, Boolean value) onChanged;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledSwitchVariant.standard => material.Switch(
        value: this.value,
        onChanged: !this.enabled ? null : (value) => this.onChanged(context, value),
      ),
    }.selfLet((it) => !this.enabled || this.tooltip == null ? it : it.withStyledTooltip(message: this.tooltip!));
  }

}

// ----------------

enum StyledRadioVariant {
  standard,
}

class StyledRadio extends StatelessWidget {

  const StyledRadio._({
    super.key,
    required this.variant,
    required this.enabled,
    required this.tooltip,
    required this.value,
    required this.onChanged,
  });

  const StyledRadio.standard({
    Key?                                         key = null,
    Boolean                                      enabled = true,
    String?                                      tooltip = null,
    required Boolean                             value,
    required Void Function(BuildContext context) onChanged,
  }) : this._(
    key: key,
    variant: StyledRadioVariant.standard,
    enabled: enabled,
    tooltip: tooltip,
    value: value,
    onChanged: onChanged,
  );

  // ----------------

  final StyledRadioVariant                  variant;
  final Boolean                             enabled;
  final String?                             tooltip;
  final Boolean                             value;
  final Void Function(BuildContext context) onChanged;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledRadioVariant.standard => RadioGroup(
        groupValue: this.value,
        onChanged: (value) => this.onChanged(context),
        child: material.Radio(
          enabled: this.enabled,
          value: true,
        ),
      ),
    }.selfLet((it) => !this.enabled || this.tooltip == null ? it : it.withStyledTooltip(message: this.tooltip!));
  }

}

// ----------------

enum StyledSliderVariant {
  standard,
}

class StyledSlider extends StatelessWidget {

  const StyledSlider._({
    super.key,
    required this.variant,
    required this.enabled,
    required this.tooltip,
    required this.minimum,
    required this.maximum,
    required this.value,
    required this.onChanged,
    required this.onChangeStart,
    required this.onChangeEnd,
  });

  const StyledSlider.standard({
    Key?                                                         key = null,
    Boolean                                                      enabled = true,
    String?                                                      tooltip = null,
    required Floater                                             minimum,
    required Floater                                             maximum,
    required Floater                                             value,
    required Void Function(BuildContext context, Floater value)  onChanged,
    required Void Function(BuildContext context, Floater value)? onChangeStart,
    required Void Function(BuildContext context, Floater value)? onChangeEnd,
  }) : this._(
    key: key,
    variant: StyledSliderVariant.standard,
    enabled: enabled,
    tooltip: tooltip,
    minimum: minimum,
    maximum: maximum,
    value: value,
    onChanged: onChanged,
    onChangeStart: onChangeStart,
    onChangeEnd: onChangeEnd,
  );

  // ----------------

  final StyledSliderVariant                                 variant;
  final Boolean                                             enabled;
  final String?                                             tooltip;
  final Floater                                             minimum;
  final Floater                                             maximum;
  final Floater                                             value;
  final Void Function(BuildContext context, Floater value)  onChanged;
  final Void Function(BuildContext context, Floater value)? onChangeStart;
  final Void Function(BuildContext context, Floater value)? onChangeEnd;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledSliderVariant.standard => material.Slider(
        min: this.minimum,
        max: this.maximum,
        value: this.value,
        onChanged: !this.enabled ? null : (value) => this.onChanged(context, value),
        onChangeStart: this.onChangeStart == null ? null : (value) => this.onChangeStart!(context, value),
        onChangeEnd: this.onChangeEnd == null ? null : (value) => this.onChangeEnd!(context, value),
      ),
    }.selfLet((it) => !this.enabled || this.tooltip == null ? it : it.withStyledTooltip(message: this.tooltip!));
  }

}

// ----------------

enum StyledInputVariant {
  filled,
  outlined,
  underlined,
}

typedef StyledInputType = TextInputType;

typedef StyledInputFormatter = TextInputFormatter;

typedef StyledInputFilteringFormatter = FilteringTextInputFormatter;

class StyledInput extends StatefulWidget {

  const StyledInput._({
    super.key,
    required this.variant,
    required this.style,
    required this.enabled,
    required this.type,
    required this.format,
    required this.hint,
    required this.prefix,
    required this.suffix,
    required this.value,
    required this.onChanged,
  });

  const StyledInput.filled({
    Key?                                                       key = null,
    TextStyle?                                                 style = null,
    Boolean                                                    enabled = true,
    required StyledInputType                                   type,
    required List<StyledInputFormatter>                        format,
    required String?                                           hint,
    required IconData?                                         prefix,
    required List<Widget>?                                     suffix,
    required String                                            value,
    required Void Function(BuildContext context, String value) onChanged,
  }) : this._(
    key: key,
    variant: StyledInputVariant.filled,
    style: style,
    enabled: enabled,
    type: type,
    format: format,
    hint: hint,
    prefix: prefix,
    suffix: suffix,
    value: value,
    onChanged: onChanged,
  );

  const StyledInput.outlined({
    Key?                                                       key = null,
    TextStyle?                                                 style = null,
    Boolean                                                    enabled = true,
    required StyledInputType                                   type,
    required List<StyledInputFormatter>                        format,
    required String?                                           hint,
    required IconData?                                         prefix,
    required List<Widget>?                                     suffix,
    required String                                            value,
    required Void Function(BuildContext context, String value) onChanged,
  }) : this._(
    key: key,
    variant: StyledInputVariant.outlined,
    style: style,
    enabled: enabled,
    type: type,
    format: format,
    hint: hint,
    prefix: prefix,
    suffix: suffix,
    value: value,
    onChanged: onChanged,
  );

  const StyledInput.underlined({
    Key?                                                       key = null,
    TextStyle?                                                 style = null,
    Boolean                                                    enabled = true,
    required StyledInputType                                   type,
    required List<StyledInputFormatter>                        format,
    required String?                                           hint,
    required IconData?                                         prefix,
    required List<Widget>?                                     suffix,
    required String                                            value,
    required Void Function(BuildContext context, String value) onChanged,
  }) : this._(
    key: key,
    variant: StyledInputVariant.underlined,
    style: style,
    enabled: enabled,
    type: type,
    format: format,
    hint: hint,
    prefix: prefix,
    suffix: suffix,
    value: value,
    onChanged: onChanged,
  );

  // ----------------

  final StyledInputVariant                                variant;
  final TextStyle?                                        style;
  final Boolean                                           enabled;
  final StyledInputType                                   type;
  final List<StyledInputFormatter>                        format;
  final String?                                           hint;
  final IconData?                                         prefix;
  final List<Widget>?                                     suffix;
  final String                                            value;
  final Void Function(BuildContext context, String value) onChanged;

  // ----------------

  @override
  createState() => _StyledInputState();

}

class _StyledInputState extends State<StyledInput> {

  late FocusNode             _focusNode;
  late TextEditingController _valueController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._focusNode = FocusNode();
    this._valueController = TextEditingController();
    this._valueController.text = this.widget.value;
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    this._valueController.text = this.widget.value;
    return;
  }

  @override
  dispose() {
    this._focusNode.dispose();
    this._valueController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var decoration = switch (this.widget.variant) {
      StyledInputVariant.filled => material.InputDecoration(
        contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
        filled: true,
        border: material.UnderlineInputBorder(),
      ),
      StyledInputVariant.outlined => material.InputDecoration(
        contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
        filled: false,
        border: material.OutlineInputBorder(),
      ),
      StyledInputVariant.underlined => material.InputDecoration(
        contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
        filled: false,
        border: material.UnderlineInputBorder(),
      ),
    };
    decoration = decoration.copyWith(
      contentPadding: decoration.contentPadding,
      filled: decoration.filled,
      border: decoration.border,
      hintText: this.widget.hint,
      prefixIcon: this.widget.prefix == null
        ? null
        : Icon(this.widget.prefix!),
      suffixIcon: this.widget.suffix == null
        ? null
        : FlexContainer.horizontal(mainStretch: false, [
          Gap.horizontal(4),
          ...this.widget.suffix!,
          Gap.horizontal(4),
        ]),
    );
    return Focus(
      focusNode: this._focusNode,
      onFocusChange: (value) async {
        if (!value) {
          this.widget.onChanged(context, this._valueController.text);
        }
      },
      child: material.TextField(
        style: this.widget.style,
        enabled: this.widget.enabled,
        keyboardType: this.widget.type,
        maxLines: this.widget.type == StyledInputType.multiline ? null : 1,
        inputFormatters: this.widget.format,
        decoration: decoration,
        readOnly: this.widget.type == StyledInputType.none,
        controller: this._valueController,
        onTapOutside: (event) async {
          this._focusNode.unfocus();
        },
      ),
    );
  }

}

// ----------------

class StyledInputCombo extends StatefulWidget {

  const StyledInputCombo._({
    super.key,
    required this.variant,
    required this.style,
    required this.enabled,
    required this.hint,
    required this.prefix,
    required this.suffix,
    required this.option,
    required this.value,
    required this.onChanged,
  });

  // TODO: variant more
  const StyledInputCombo.underlined({
    Key?                                                        key = null,
    TextStyle?                                                  style = null,
    Boolean                                                     enabled = true,
    required String?                                            hint,
    required IconData?                                          prefix,
    required List<Widget>?                                      suffix,
    required List<(Object, String)>                             option,
    required String                                             value,
    required Void Function(BuildContext context, Object? value) onChanged,
  }) : this._(
    key: key,
    variant: StyledInputVariant.underlined,
    style: style,
    enabled: enabled,
    hint: hint,
    prefix: prefix,
    suffix: suffix,
    option: option,
    value: value,
    onChanged: onChanged,
  );

  // ----------------

  final StyledInputVariant                                 variant;
  final TextStyle?                                         style;
  final Boolean                                            enabled;
  final String?                                            hint;
  final IconData?                                          prefix;
  final List<Widget>?                                      suffix;
  final List<(Object, String)>                             option;
  final String                                             value;
  final Void Function(BuildContext context, Object? value) onChanged;

  // ----------------

  @override
  createState() => _StyledInputComboState();

}

class _StyledInputComboState extends State<StyledInputCombo> {

  late FocusNode             _focusNode;
  late TextEditingController _valueController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._focusNode = FocusNode();
    this._valueController = TextEditingController();
    this._valueController.text = this.widget.value;
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    this._valueController.text = this.widget.value;
    return;
  }

  @override
  dispose() {
    this._focusNode.dispose();
    this._valueController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    var theme = material.Theme.of(context);
    var decoration = switch (this.widget.variant) {
      StyledInputVariant.filled => material.InputDecoration(
        contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
        filled: true,
        border: material.UnderlineInputBorder(),
      ),
      StyledInputVariant.outlined => material.InputDecoration(
        contentPadding: EdgeInsets.fromLTRB(12, 16, 12, 16),
        filled: false,
        border: material.OutlineInputBorder(),
      ),
      StyledInputVariant.underlined => material.InputDecoration(
        contentPadding: EdgeInsets.fromLTRB(8, 12, 8, 12),
        filled: false,
        border: material.UnderlineInputBorder(),
      ),
    };
    decoration = decoration.copyWith(
      contentPadding: decoration.contentPadding,
      filled: decoration.filled,
      border: decoration.border,
      hintText: this.widget.hint,
      prefixIcon: this.widget.prefix == null
        ? null
        : Icon(this.widget.prefix!),
      suffixIcon: this.widget.suffix == null
        ? null
        : FlexContainer.horizontal(mainStretch: false, [
          Gap.horizontal(4),
          ...this.widget.suffix!,
          Gap.horizontal(4),
        ]),
    );
    return LayoutBuilder(
      builder: (context, constraints) => material.MenuAnchor(
        style: material.MenuStyle(
          minimumSize: WidgetStatePropertyAll(Size(constraints.maxWidth + 8, 0)),
          maximumSize: WidgetStatePropertyAll(Size(constraints.maxWidth + 8, Floater.infinity)),
        ),
        crossAxisUnconstrained: false,
        alignmentOffset: Offset(-4, 0),
        menuChildren: [
          if (this.widget.option.isEmpty)
            Gap.vertical(16),
          ...this.widget.option.map((value) => material.MenuItemButton(
            style: material.ButtonStyle(
              backgroundColor: WidgetStatePropertyAll(value.$2 != this.widget.value ? null : theme.colorScheme.onSurface.withValues(alpha: 0.12)),
            ),
            child: ConstrainedBox(
              constraints: BoxConstraints(maxWidth: constraints.maxWidth - 16),
              child: StyledListTile.standardTight(
                dense: true,
                content: DefaultTextStyle(
                  style: theme.textTheme.bodyLarge!,
                  child: StyledText.custom(
                    value.$2,
                    overflow: TextOverflow.clip,
                    style: this.widget.style,
                  ),
                ),
              ),
            ),
            onPressed: () async {
              this.widget.onChanged(context, value.$1);
              this._focusNode.unfocus();
            },
          )),
        ],
        builder: (context, controller, child) => material.TextField(
          style: this.widget.style,
          enabled: this.widget.enabled,
          keyboardType: StyledInputType.none,
          inputFormatters: [],
          decoration: decoration,
          readOnly: true,
          controller: this._valueController,
          focusNode: this._focusNode,
          onTap: () async {
            if (controller.isOpen) {
              controller.close();
            }
            else {
              controller.open();
            }
          },
          onTapOutside: (event) async {
            this._focusNode.unfocus();
          },
        ),
      ),
    );
  }

}

// #endregion

// #region container

enum StyledCardVariant {
  elevated,
  filled,
  outlined,
}

class StyledCard extends StatelessWidget {

  const StyledCard._({
    super.key,
    required this.variant,
    required this.margin,
    required this.color,
    required this.content,
  });

  const StyledCard.elevated({
    Key?               key = null,
    EdgeInsetsGeometry margin = EdgeInsets.zero,
    Color?             color = null,
    required Widget    content,
  }) : this._(
    key: key,
    variant: StyledCardVariant.elevated,
    margin: margin,
    color: color,
    content: content,
  );

  const StyledCard.filled({
    Key?               key = null,
    EdgeInsetsGeometry margin = EdgeInsets.zero,
    Color?             color = null,
    required Widget    content,
  }) : this._(
    key: key,
    variant: StyledCardVariant.filled,
    margin: margin,
    color: color,
    content: content,
  );

  const StyledCard.outlined({
    Key?               key = null,
    EdgeInsetsGeometry margin = EdgeInsets.zero,
    Color?             color = null,
    required Widget    content,
  }) : this._(
    key: key,
    variant: StyledCardVariant.outlined,
    margin: margin,
    color: color,
    content: content,
  );

  // ----------------

  final StyledCardVariant  variant;
  final EdgeInsetsGeometry margin;
  final Color?             color;
  final Widget             content;

  // ----------------

  @override
  build(context) {
    return switch (variant) {
      StyledCardVariant.elevated => material.Card(
        clipBehavior: Clip.antiAliasWithSaveLayer,
        margin: this.margin,
        color: this.color,
        child: this.content,
      ),
      StyledCardVariant.filled => material.Card.filled(
        clipBehavior: Clip.antiAliasWithSaveLayer,
        margin: this.margin,
        color: this.color,
        child: this.content,
      ),
      StyledCardVariant.outlined => material.Card.outlined(
        clipBehavior: Clip.antiAliasWithSaveLayer,
        margin: this.margin,
        color: this.color,
        child: this.content,
      ),
    };
  }

}

// ----------------

enum StyledListTileVariant {
  standard,
}

class StyledListTile extends StatelessWidget {

  const StyledListTile._({
    super.key,
    required this.variant,
    required this.enabled,
    required this.dense,
    required this.padding,
    required this.leading,
    required this.content,
    required this.trailing,
    required this.onPressed,
    required this.onLongPressed,
  });

  const StyledListTile.standard({
    Key?                                 key = null,
    Boolean                              enabled = true,
    Boolean                              dense = false,
    Widget?                              leading = null,
    Widget?                              content = null,
    Widget?                              trailing = null,
    Void Function(BuildContext context)? onPressed = null,
    Void Function(BuildContext context)? onLongPressed = null,
  }) : this._(
    key: key,
    variant: StyledListTileVariant.standard,
    enabled: enabled,
    dense: dense,
    padding: null,
    leading: leading,
    content: content,
    trailing: trailing,
    onPressed: onPressed,
    onLongPressed: onLongPressed,
  );

  // TODO: remove?
  const StyledListTile.standardTight({
    Key?                                 key = null,
    Boolean                              enabled = true,
    Boolean                              dense = false,
    Widget?                              leading = null,
    Widget?                              content = null,
    Widget?                              trailing = null,
    Void Function(BuildContext context)? onPressed = null,
    Void Function(BuildContext context)? onLongPressed = null,
  }) : this._(
    key: key,
    variant: StyledListTileVariant.standard,
    enabled: enabled,
    dense: dense,
    padding: EdgeInsets.zero,
    leading: leading,
    content: content,
    trailing: trailing,
    onPressed: onPressed,
    onLongPressed: onLongPressed,
  );

  // TODO: remove?
  const StyledListTile.standardCustom({
    Key?                                 key = null,
    Boolean                              enabled = true,
    Boolean                              dense = false,
    EdgeInsetsGeometry?                  padding = null,
    Widget?                              leading = null,
    Widget?                              content = null,
    Widget?                              trailing = null,
    Void Function(BuildContext context)? onPressed = null,
    Void Function(BuildContext context)? onLongPressed = null,
  }) : this._(
    key: key,
    variant: StyledListTileVariant.standard,
    enabled: enabled,
    dense: dense,
    padding: padding,
    leading: leading,
    content: content,
    trailing: trailing,
    onPressed: onPressed,
    onLongPressed: onLongPressed,
  );

  // ----------------

  final StyledListTileVariant                variant;
  final Boolean                              enabled;
  final Boolean                              dense;
  final EdgeInsetsGeometry?                  padding;
  final Widget?                              leading;
  final Widget?                              content;
  final Widget?                              trailing;
  final Void Function(BuildContext context)? onPressed;
  final Void Function(BuildContext context)? onLongPressed;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledListTileVariant.standard => material.ListTile(
        enabled: this.enabled,
        dense: this.dense,
        contentPadding: this.padding,
        leading: this.leading,
        title: this.content,
        trailing: this.trailing,
        onTap: this.onPressed == null ? null : () => this.onPressed!(context),
        onLongPress: this.onLongPressed == null ? null : () => this.onLongPressed!(context),
      ),
    };
  }

}

// #endregion

// #region scroll

enum StyledScrollBarVariant {
  standard,
}

class StyledScrollBar extends StatelessWidget {

  const StyledScrollBar._({
    super.key,
    required this.variant,
    required this.controller,
    required this.child,
  });

  const StyledScrollBar.standard({
    Key?                      key = null,
    required ScrollController controller,
    required Widget           child,
  }) : this._(
    key: key,
    variant: StyledScrollBarVariant.standard,
    controller: controller,
    child: child,
  );

  // ----------------

  final StyledScrollBarVariant variant;
  final ScrollController       controller;
  final Widget                 child;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledScrollBarVariant.standard => material.Scrollbar(
        interactive: true,
        controller: this.controller,
        child: this.child,
      ),
    };
  }

}

extension StyledScrollBarExtension<TType extends Widget> on TType {

  StyledScrollBar withStyledScrollBar({
    required ScrollController controller,
  }) {
    return StyledScrollBar.standard(
      controller: controller,
      child: this,
    );
  }

}

// #endregion

// #region layout

enum StyledTitleBarVariant {
  standard,
}

class StyledTitleBar extends StatelessWidget {

  const StyledTitleBar._({
    super.key,
    required this.variant,
    required this.title,
    required this.leading,
  });

  const StyledTitleBar.standard({
    Key?            key = null,
    Boolean         enabled = true,
    required String title,
    required Widget leading,
  }) : this._(
    key: key,
    variant: StyledTitleBarVariant.standard,
    title: title,
    leading: leading,
  );

  // ----------------

  final StyledTitleBarVariant variant;
  final String                title;
  final Widget                leading;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledTitleBarVariant.standard => material.AppBar(
        systemOverlayStyle: SystemOverlayHelper.query(material.Theme.of(context).colorScheme.brightness),
        centerTitle: false,
        elevation: 3,
        scrolledUnderElevation: 3,
        backgroundColor: ColorSet.transparent,
        automaticallyImplyLeading: false,
        titleSpacing: 0,
        title: FlexContainer.horizontal([
          Gap.horizontal(8),
          this.leading,
          Gap.horizontal(12),
          StyledText.custom(
            this.title,
            variant: StyledTextVariant.titleLarge,
          ).withFlexExpanded(),
          Gap.horizontal(8),
        ]),
      ),
    };
  }

}

// ----------------

enum StyledBottomBarVariant {
  standard,
}

class StyledBottomBar extends StatelessWidget {

  const StyledBottomBar._({
    super.key,
    required this.variant,
    required this.primary,
    required this.secondary,
  });

  const StyledBottomBar.standard({
    Key?                      key = null,
    required Widget?          primary,
    required Iterable<Widget> secondary,
  }) : this._(
    key: key,
    variant: StyledBottomBarVariant.standard,
    primary: primary,
    secondary: secondary,
  );

  // ----------------

  final StyledBottomBarVariant variant;
  final Widget?                primary;
  final Iterable<Widget>       secondary;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledBottomBarVariant.standard => material.BottomAppBar(
        child: FlexContainer.horizontal([
          FlexContainer.horizontal(this.secondary).withFlexExpanded(),
          if (this.primary != null)
            Gap.horizontal(16),
          if (this.primary != null)
            this.primary!,
        ]),
      ),
    };
  }

}

// ----------------

enum StyledNavigationDrawerVariant {
  standard,
}

class StyledNavigationDrawer extends StatelessWidget {

  const StyledNavigationDrawer._({
    super.key,
    required this.variant,
    required this.children,
  });

  const StyledNavigationDrawer.standard({
    Key?                      key = null,
    required Iterable<Widget> children,
  }) : this._(
    key: key,
    variant: StyledNavigationDrawerVariant.standard,
    children: children,
  );

  // ----------------

  final StyledNavigationDrawerVariant variant;
  final Iterable<Widget>              children;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledNavigationDrawerVariant.standard => material.NavigationDrawer(
        children: this.children.toList(),
      ),
    };
  }

}

extension StyledNavigationDrawerExtension<TType extends Widget> on TType {

  static Future<Void> show(
    BuildContext context,
  ) async {
    material.Scaffold.of(context).openDrawer();
    return;
  }

}

// #endregion

// #region supplementary

enum StyledMenuItemVariant {
  standard,
}

class StyledMenuItem<TValue> {

  const StyledMenuItem._({
    required this.variant,
    required this.enabled,
    required this.value,
    required this.leading,
    required this.content,
    required this.trailing,
  });

  const StyledMenuItem.standard({
    Boolean         enabled = true,
    required TValue value,
    Widget?         leading = null,
    required Widget content,
    Widget?         trailing = null,
  }) : this._(
    variant: StyledMenuItemVariant.standard,
    enabled: enabled,
    value: value,
    leading: leading,
    content: content,
    trailing: trailing,
  );

  // ----------------

  final StyledMenuItemVariant variant;
  final Boolean               enabled;
  final TValue                value;
  final Widget?               leading;
  final Widget                content;
  final Widget?               trailing;

}

// ----------------

enum StyledMenuVariant {
  standard,
}

enum StyledMenuPosition {
  under,
}

class StyledMenu<TValue> {

  const StyledMenu._({
    required this.variant,
    required this.position,
    required this.children,
  });

  const StyledMenu.standard({
    required StyledMenuPosition                position,
    required Iterable<StyledMenuItem<TValue>?> children,
  }) : this._(
    variant: StyledMenuVariant.standard,
    position: position,
    children: children,
  );

  // ----------------

  final StyledMenuVariant                 variant;
  final StyledMenuPosition                position;
  final Iterable<StyledMenuItem<TValue>?> children;

}

extension StyledMenuExtension<TType extends Widget> on TType {

  static Future<TResult?> show<TResult>(
    BuildContext        context,
    StyledMenu<TResult> menu,
  ) async {
    var button = context.findRenderObject()!.as<RenderBox>();
    var overlay = Overlay.of(context).context.findRenderObject()!.as<RenderBox>();
    var position = RelativeRect.fromRect(
      Rect.fromPoints(
        button.localToGlobal(Offset(0, overlay.size.height), ancestor: overlay),
        button.localToGlobal(button.size.bottomRight(Offset.zero), ancestor: overlay),
      ),
      Offset.zero & overlay.size,
    );
    return await material.showMenu(
      context: context,
      position: position,
      items: menu.children.length == 0
        ? [
          material.PopupMenuItem(
            height: 16,
            enabled: false,
            value: null,
            child: null,
          ),
        ]
        : menu.children.map((item) => item == null
          ? material.PopupMenuDivider()
          : material.PopupMenuItem(
            enabled: item.enabled,
            value: item.value,
            child: StyledListTile.standardCustom(
              padding: EdgeInsets.zero,
              enabled: item.enabled,
              dense: true,
              leading: item.leading,
              content: item.content,
              trailing: DefaultTextStyle(
                style: material.Theme.of(context).textTheme.labelSmall!.copyWith(
                  color: material.Theme.of(context).colorScheme.onSurfaceVariant,
                ),
                child: item.trailing ?? Box.none(),
              ),
            ),
          ).as<material.PopupMenuEntry<TResult>>()).toList(),
    );
  }

}

// ----------------

enum StyledFullDialogVariant {
  standard,
}

class StyledFullDialog extends StatefulWidget {

  const StyledFullDialog._({
    super.key,
    required this.variant,
    required this.title,
    required this.contentBuilder,
  });

  const StyledFullDialog.standard({
    Key?                                                                                              key = null,
    required String                                                                                   title,
    required Iterable<Widget> Function(BuildContext context, Void Function(Void Function()) setState) contentBuilder,
  }) : this._(
    key: key,
    variant: StyledFullDialogVariant.standard,
    title: title,
    contentBuilder: contentBuilder,
  );

  // ----------------

  final StyledFullDialogVariant                                                                  variant;
  final String                                                                                   title;
  final Iterable<Widget> Function(BuildContext context, Void Function(Void Function()) setState) contentBuilder;

  // ----------------

  @override
  createState() => _StyledFullDialogState();

}

class _StyledFullDialogState extends State<StyledFullDialog> {

  late ScrollController _scrollController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._scrollController = ScrollController();
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    this._scrollController.dispose();
    this._scrollController = ScrollController();
    return;
  }

  @override
  dispose() {
    this._scrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    return switch (this.widget.variant) {
      StyledFullDialogVariant.standard => material.Dialog.fullscreen(
        child: StyledScaffold.standard(
          title: StyledTitleBar.standard(
            title: this.widget.title,
            leading: Builder(
              builder: (context) => StyledIconButton.standard(
                tooltip: 'Back',
                icon: Icon(IconSet.arrow_back),
                onPressed: (context) async {
                  Navigator.pop(context);
                },
              ),
            ),
          ),
          body: FlexContainer.vertical([
            StatefulBuilder(
              builder: (context, setStateForPanel) => FlexContainer.vertical(mainStretch: false, [
                ...this.widget.contentBuilder(context, setStateForPanel),
                Gap.vertical(MediaQuery.viewPaddingOf(context).bottom),
              ]),
            ).withScrollableArea(
              vertical: this._scrollController,
            ).withStyledScrollBar(
              controller: this._scrollController,
            ).withStyledMaterial(
            ).withFlexExpanded(),
          ]),
        ),
      ),
    };
  }

}

extension StyledFullDialogExtension<TType extends Widget> on TType {

  static Future<TResult?> show<TResult>(
    BuildContext     context,
    StyledFullDialog dialog,
  ) async {
    return await material.showDialog<TResult>(
      useSafeArea: false,
      context: context,
      builder: (context) => dialog,
    );
  }

}

// ----------------

enum StyledModalDialogVariant {
  standard,
}

class StyledModalDialog extends StatefulWidget {

  const StyledModalDialog._({
    super.key,
    required this.variant,
    required this.title,
    required this.contentBuilder,
    required this.actionBuilder,
  });

  const StyledModalDialog.standard({
    Key?                                                                                              key = null,
    required String                                                                                   title,
    required Iterable<Widget> Function(BuildContext context, Void Function(Void Function()) setState) contentBuilder,
    required Iterable<Widget> Function(BuildContext context)?                                         actionBuilder,
  }) : this._(
    key: key,
    variant: StyledModalDialogVariant.standard,
    title: title,
    contentBuilder: contentBuilder,
    actionBuilder: actionBuilder,
  );

  // ----------------

  final StyledModalDialogVariant                                                                 variant;
  final String                                                                                   title;
  final Iterable<Widget> Function(BuildContext context, Void Function(Void Function()) setState) contentBuilder;
  final Iterable<Widget> Function(BuildContext context)?                                         actionBuilder;

  // ----------------

  @override
  createState() => _StyledModalDialogState();

}

class _StyledModalDialogState extends State<StyledModalDialog> {

  late ScrollController _scrollController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._scrollController = ScrollController();
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    this._scrollController.dispose();
    this._scrollController = ScrollController();
    return;
  }

  @override
  dispose() {
    this._scrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    return switch (this.widget.variant) {
      StyledModalDialogVariant.standard => material.AlertDialog(
        title: StyledText.inherit(this.widget.title),
        content: StatefulBuilder(
          builder: (context, setStateForPanel) => FlexContainer.vertical(mainStretch: false, [
            ...this.widget.contentBuilder(context, setStateForPanel),
          ]),
        ).withScrollableArea(
          vertical: this._scrollController,
        ).withStyledScrollBar(
          controller: this._scrollController,
        ).withStyledMaterial(
        ),
        actions: this.widget.actionBuilder == null
          ? [
            StyledButton.text(
              content: StyledText.inherit('Close'),
              onPressed: (context) => Navigator.pop(context),
            ),
          ]
          : this.widget.actionBuilder!(context).toList(),
      ),
    };
  }

}

extension StyledModalDialogExtension<TType extends Widget> on TType {

  static Future<TResult?> show<TResult>(
    BuildContext      context,
    StyledModalDialog dialog,
  ) async {
    return await material.showDialog<TResult>(
      context: context,
      builder: (context) => dialog,
    );
  }

}

// ----------------

enum StyledModalBottomSheetVariant {
  standard,
}

class StyledModalBottomSheet extends StatefulWidget {

  const StyledModalBottomSheet._({
    super.key,
    required this.variant,
    required this.title,
    required this.contentBuilder,
  });

  const StyledModalBottomSheet.standard({
    Key?                                                                                              key = null,
    required String                                                                                   title,
    required Iterable<Widget> Function(BuildContext context, Void Function(Void Function()) setState) contentBuilder,
  }) : this._(
    key: key,
    variant: StyledModalBottomSheetVariant.standard,
    title: title,
    contentBuilder: contentBuilder,
  );

  // ----------------

  final StyledModalBottomSheetVariant                                                            variant;
  final String                                                                                   title;
  final Iterable<Widget> Function(BuildContext context, Void Function(Void Function()) setState) contentBuilder;

  // ----------------

  @override
  createState() => _StyledModalBottomSheetState();

}

class _StyledModalBottomSheetState extends State<StyledModalBottomSheet> {

  late ScrollController _scrollController;

  // ----------------

  @override
  initState() {
    super.initState();
    this._scrollController = ScrollController();
    return;
  }

  @override
  didUpdateWidget(oldWidget) {
    super.didUpdateWidget(oldWidget);
    this._scrollController.dispose();
    this._scrollController = ScrollController();
    return;
  }

  @override
  dispose() {
    this._scrollController.dispose();
    super.dispose();
    return;
  }

  @override
  build(context) {
    return switch (this.widget.variant) {
      StyledModalBottomSheetVariant.standard => FlexContainer.vertical([
        BoxContainer.of(
          padding: EdgeInsets.fromLTRB(24, 12, 24, 12),
          child: FlexContainer.horizontal([
            StyledText.custom(
              this.widget.title,
              style: material.Theme.of(context).textTheme.titleMedium!.copyWith(
                color: material.Theme.of(context).colorScheme.primary,
              ),
            ).withFlexExpanded(),
          ]),
        ),
        StyledDivider.minimal(indent: 12),
        StatefulBuilder(
          builder: (context, setStateForPanel) => FlexContainer.vertical(mainStretch: false, [
            ...this.widget.contentBuilder(context, setStateForPanel),
            Gap.vertical(MediaQuery.viewPaddingOf(context).bottom),
          ]),
        ).withScrollableArea(
          vertical: this._scrollController,
        ).withStyledScrollBar(
          controller: this._scrollController,
        ).withStyledMaterial(
        ).withFlexExpanded(),
      ]),
    };
  }

}

extension StyledBottomSheetExtension<TType extends Widget> on TType {

  static Future<TResult?> show<TResult>(
    BuildContext           context,
    StyledModalBottomSheet sheet,
  ) async {
    return await material.showModalBottomSheet<TResult>(
      context: context,
      elevation: 3,
      builder: (context) => sheet,
    );
  }

}

// #endregion

// #region framework

enum StyledScaffoldVariant {
  standard,
}

class StyledScaffold extends StatelessWidget {

  const StyledScaffold._({
    super.key,
    required this.variant,
    required this.title,
    required this.body,
    required this.drawer,
  });

  const StyledScaffold.standard({
    Key?            key = null,
    required Widget title,
    required Widget body,
    Widget?         drawer = null,
  }) : this._(
    key: key,
    variant: StyledScaffoldVariant.standard,
    title: title,
    body: body,
    drawer: drawer,
  );

  // ----------------

  final StyledScaffoldVariant variant;
  final Widget                title;
  final Widget                body;
  final Widget?               drawer;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledScaffoldVariant.standard => material.Scaffold(
        appBar: PreferredSize(
          preferredSize: Size.fromHeight(material.kToolbarHeight),
          child: this.title,
        ),
        drawer: this.drawer,
        body: this.body,
      ),
    };
  }

}

// ----------------

enum StyledApplicationVariant {
  standard,
}

class StyledApplication extends StatelessWidget {

  const StyledApplication._({
    super.key,
    required this.variant,
    required this.navigatorKey,
    required this.themeMode,
    required this.themeColorState,
    required this.themeColorLight,
    required this.themeColorDark,
    required this.themeFontFamily,
    required this.home,
  });

  const StyledApplication.standard({
    Key?                               key = null,
    required GlobalKey<NavigatorState> navigatorKey,
    required StyledThemeMode           themeMode,
    required Boolean                   themeColorState,
    required Color                     themeColorLight,
    required Color                     themeColorDark,
    required List<String>              themeFontFamily,
    required Widget                    home,
  }) : this._(
    key: key,
    variant: StyledApplicationVariant.standard,
    navigatorKey: navigatorKey,
    themeMode: themeMode,
    themeColorState: themeColorState,
    themeColorLight: themeColorLight,
    themeColorDark: themeColorDark,
    themeFontFamily: themeFontFamily,
    home: home,
  );

  // ----------------

  final StyledApplicationVariant  variant;
  final GlobalKey<NavigatorState> navigatorKey;
  final StyledThemeMode           themeMode;
  final Boolean                   themeColorState;
  final Color                     themeColorLight;
  final Color                     themeColorDark;
  final List<String>              themeFontFamily;
  final Widget                    home;

  // ----------------

  @override
  build(context) {
    return switch (this.variant) {
      StyledApplicationVariant.standard => lib.DynamicColorBuilder(
        builder: (lightColor, darkColor) {
          // fix for dynamic color error on flutter 3.22+, see https://github.com/material-foundation/flutter-packages/issues/582#issuecomment-2209591668
          if (SystemChecker.isAndroid) {
            if (lightColor != null) {
              lightColor = material.ColorScheme.fromSeed(seedColor: lightColor.primary, brightness: Brightness.light);
            }
            if (darkColor != null) {
              darkColor = material.ColorScheme.fromSeed(seedColor: darkColor.primary, brightness: Brightness.dark);
            }
          }
          return material.MaterialApp(
            navigatorKey: this.navigatorKey,
            theme: material.ThemeData(
              materialTapTargetSize: material.MaterialTapTargetSize.shrinkWrap,
              visualDensity: material.VisualDensity.standard,
              brightness: Brightness.light,
              colorScheme: !this.themeColorState ? lightColor : material.ColorScheme.fromSeed(seedColor:this.themeColorLight, brightness: Brightness.light),
              fontFamily: '',
              fontFamilyFallback: [...this.themeFontFamily],
              tooltipTheme: material.TooltipTheme.of(context).copyWith(
                waitDuration: Duration(milliseconds: 1000),
              ),
              progressIndicatorTheme: material.ProgressIndicatorThemeData(
                year2023: false, // ignore: deprecated_member_use
              ),
              sliderTheme: material.SliderThemeData(
                year2023: false, // ignore: deprecated_member_use
              ),
            ),
            darkTheme: material.ThemeData(
              materialTapTargetSize: material.MaterialTapTargetSize.shrinkWrap,
              visualDensity: material.VisualDensity.standard,
              brightness: Brightness.dark,
              colorScheme: !this.themeColorState ? darkColor : material.ColorScheme.fromSeed(seedColor: this.themeColorDark, brightness: Brightness.dark),
              fontFamily: '',
              fontFamilyFallback: [...this.themeFontFamily],
              tooltipTheme: material.TooltipTheme.of(context).copyWith(
                waitDuration: Duration(milliseconds: 1000),
              ),
              progressIndicatorTheme: material.ProgressIndicatorThemeData(
                year2023: false, // ignore: deprecated_member_use
              ),
              sliderTheme: material.SliderThemeData(
                year2023: false, // ignore: deprecated_member_use
              ),
            ),
            themeMode: material.ThemeMode.values[this.themeMode.index],
            scrollBehavior: material.MaterialScrollBehavior().copyWith(scrollbars: false),
            title: ApplicationInformation.name,
            home: Builder(
              builder: (context) {
                SystemOverlayHelper.apply(material.Theme.of(context).colorScheme.brightness);
                return this.home;
              },
            ),
          );
        },
      ),
    };
  }

}

// #endregion
