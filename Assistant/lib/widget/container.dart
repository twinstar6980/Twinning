import '/common.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter/material.dart' as material;
import 'package:single_child_two_dimensional_scroll_view/single_child_two_dimensional_scroll_view.dart' as lib;

// ----------------

// #region layout

class Gap extends StatelessWidget {

  const Gap._({
    super.key,
    required this.width,
    required this.height,
  });

  const Gap.horizontal(
    Floater size, {
    Key?    key = null,
  }) : this._(
    key: key,
    width: size,
    height: 0.0,
  );

  const Gap.vertical(
    Floater size, {
    Key?    key = null,
  }) : this._(
    key: key,
    width: 0.0,
    height: size,
  );

  // ----------------

  final Floater width;
  final Floater height;

  // ----------------

  @override
  build(context) {
    return SizedBox(
      width: this.width,
      height: this.height,
    );
  }

}

// ----------------

typedef BoxContainerAlign = Alignment;

class BoxContainer extends StatelessWidget {

  const BoxContainer._({
    super.key,
    required this.constraints,
    required this.margin,
    required this.padding,
    required this.align,
    required this.transform,
    required this.color,
    required this.colorFilter,
    required this.child,
  });

  const BoxContainer.of({
    Key?                key = null,
    BoxConstraints?     constraints = null,
    EdgeInsetsGeometry? margin = null,
    EdgeInsetsGeometry? padding = null,
    BoxContainerAlign?  align = null,
    Matrix4?            transform = null,
    Color?              color = null,
    ColorFilter?        colorFilter = null,
    required Widget?    child,
  }) : this._(
    key: key,
    constraints: constraints,
    margin: margin,
    padding: padding,
    align: align,
    transform: transform,
    color: color,
    colorFilter: colorFilter,
    child: child,
  );

  const BoxContainer.none({
    Key? key = null,
  }) : this._(
    key: key,
    constraints: const .tightFor(width: 0, height: 0),
    margin: null,
    padding: null,
    align: null,
    transform: null,
    color: null,
    colorFilter: null,
    child: null,
  );

  // ----------------

  final BoxConstraints?     constraints;
  final EdgeInsetsGeometry? margin;
  final EdgeInsetsGeometry? padding;
  final BoxContainerAlign?  align;
  final Matrix4?            transform;
  final Color?              color;
  final ColorFilter?        colorFilter;
  final Widget?             child;

  // ----------------

  @override
  build(context) {
    return Container(
      constraints: this.constraints,
      margin: this.margin,
      padding: this.padding,
      alignment: this.align,
      transform: this.transform,
      color: this.color,
      child: this.child,
    ).selfLet((it) => this.colorFilter == null ? it : ColorFiltered(colorFilter: this.colorFilter!, child: it));
  }

}

// ----------------

typedef StackContainerFit = StackFit;

class StackContainer extends StatelessWidget {

  const StackContainer._({
    super.key,
    required this.fit,
    required this.index,
    required this.children,
  });

  const StackContainer.of(
    Iterable<Widget>  children, {
    Key?              key = null,
    StackContainerFit fit = .loose,
  }) : this._(
    key: key,
    fit: fit,
    index: null,
    children: children,
  );

  const StackContainer.at(
    Integer           index,
    Iterable<Widget>  children, {
    Key?              key = null,
    StackContainerFit fit = .loose,
  }) : this._(
    key: key,
    fit: fit,
    index: index,
    children: children,
  );

  // ----------------

  final StackContainerFit fit;
  final Integer?          index;
  final Iterable<Widget>  children;

  // ----------------

  @override
  build(context) {
    return this.index == null
      ? Stack(
        children: this.children.toList(),
      )
      : IndexedStack(
        index: this.index!,
        children: this.children.toList(),
      );
  }

}

// ----------------

class FlexExpanded extends StatelessWidget {

  const FlexExpanded._({
    super.key,
    required this.weight,
    required this.child,
  });

  const FlexExpanded.of(
    Widget  child, {
    Key?    key = null,
    Integer weight = 1,
  }) : this._(
    key: key,
    weight: weight,
    child: child,
  );

  // ----------------

  final Integer weight;
  final Widget  child;

  // ----------------

  @override
  build(context) {
    return Expanded(
      flex: this.weight,
      child: this.child,
    );
  }

}

extension FlexExpandedWidgetExtension on Widget {

  FlexExpanded withFlexExpanded({
    Integer? weight = null,
  }) {
    weight ??= 1;
    return .of(
      this,
      weight: weight,
    );
  }

}

// ----------------

typedef FlexContainerDirection = Axis;

typedef FlexContainerCrossAlign = CrossAxisAlignment;

class FlexContainer extends StatelessWidget {

  const FlexContainer._({
    super.key,
    required this.direction,
    required this.mainStretch,
    required this.crossAlign,
    required this.textBaseline,
    required this.children,
  });

  const FlexContainer.horizontal(
    Iterable<Widget>        children, {
    Key?                    key = null,
    Boolean                 mainStretch = true,
    FlexContainerCrossAlign crossAlign = .center,
    TextBaseline?           textBaseline = null,
  }) : this._(
    key: key,
    direction: .horizontal,
    mainStretch: mainStretch,
    crossAlign: crossAlign,
    textBaseline: textBaseline,
    children: children,
  );

  const FlexContainer.vertical(
    Iterable<Widget>        children, {
    Key?                    key = null,
    Boolean                 mainStretch = true,
    FlexContainerCrossAlign crossAlign = .center,
    TextBaseline?           textBaseline = null,
  }) : this._(
    key: key,
    direction: .vertical,
    mainStretch: mainStretch,
    crossAlign: crossAlign,
    textBaseline: textBaseline,
    children: children,
  );

  // ----------------

  final FlexContainerDirection  direction;
  final Boolean                 mainStretch;
  final FlexContainerCrossAlign crossAlign;
  final TextBaseline?           textBaseline;
  final Iterable<Widget>        children;

  // ----------------

  @override
  build(context) {
    return Flex(
      direction: this.direction,
      mainAxisSize: !this.mainStretch ? .min : .max,
      mainAxisAlignment: .start,
      crossAxisAlignment: this.textBaseline != null ? .baseline : this.crossAlign,
      textBaseline: this.textBaseline,
      children: this.children.toList(),
    );
  }

}

// #endregion

// #region feature

class VisibilityArea extends StatelessWidget {

  const VisibilityArea._({
    super.key,
    required this.enabled,
    required this.child,
  });

  const VisibilityArea.of({
    Key?            key = null,
    Boolean         enabled = true,
    required Widget child,
  }) : this._(
    key: key,
    enabled: enabled,
    child: child,
  );

  // ----------------

  final Boolean enabled;
  final Widget  child;

  // ----------------

  @override
  build(context) {
    return Visibility(
      visible: this.enabled,
      child: this.child,
    );
  }

}

extension VisibilityAreaWidgetExtension on Widget {

  VisibilityArea withVisibilityArea({
    Boolean? enabled = null,
  }) {
    enabled ??= true;
    return .of(
      enabled: enabled,
      child: this,
    );
  }

}

// ----------------

class ImpenetrableArea extends StatelessWidget {

  const ImpenetrableArea._({
    super.key,
    required this.child,
  });

  const ImpenetrableArea.of({
    Key?            key = null,
    required Widget child,
  }) : this._(
    key: key,
    child: child,
  );

  // ----------------

  final Widget child;

  // ----------------

  @override
  build(context) {
    return IgnorePointer(
      ignoring: true,
      child: this.child,
    );
  }

}

extension ImpenetrableAreaWidgetExtension on Widget {

  ImpenetrableArea withImpenetrableArea(
  ) {
    return .of(
      child: this,
    );
  }

}

// ----------------

class SelectableArea extends StatelessWidget {

  const SelectableArea._({
    super.key,
    required this.child,
  });

  const SelectableArea.of({
    Key?            key = null,
    required Widget child,
  }) : this._(
    key: key,
    child: child,
  );

  // ----------------

  final Widget child;

  // ----------------

  @override
  build(context) {
    return material.SelectionArea(
      onSelectionChanged: null,
      child: this.child,
    );
  }

}

extension SelectableAreaWidgetExtension on Widget {

  SelectableArea withSelectableArea(
  ) {
    return .of(
      child: this,
    );
  }

}

// ----------------

class ScrollableArea extends StatelessWidget {

  const ScrollableArea._({
    super.key,
    required this.horizontal,
    required this.vertical,
    required this.child,
  });

  const ScrollableArea.of({
    Key?              key,
    ScrollController? horizontal = null,
    ScrollController? vertical = null,
    required Widget   child,
  }) : this._(
    key: key,
    horizontal: horizontal,
    vertical: vertical,
    child: child,
  );

  // ----------------

  final ScrollController? horizontal;
  final ScrollController? vertical;
  final Widget            child;

  // ----------------

  @override
  build(context) {
    if (this.horizontal == null && this.vertical == null) {
      return this.child;
    }
    if (this.horizontal != null && this.vertical == null) {
      return SingleChildScrollView(
        scrollDirection: .horizontal,
        controller: this.horizontal,
        child: this.child,
      );
    }
    if (this.horizontal == null && this.vertical != null) {
      return SingleChildScrollView(
        scrollDirection: .vertical,
        controller: this.vertical,
        child: this.child,
      );
    }
    if (this.horizontal != null && this.vertical != null) {
      return lib.SingleChildTwoDimensionalScrollView(
        horizontalController: this.horizontal,
        verticalController: this.vertical,
        child: this.child,
      );
    }
    throw UnreachableException();
  }

}

extension ScrollAreaWidgetExtension on Widget {

  ScrollableArea withScrollableArea({
    ScrollController? horizontal = null,
    ScrollController? vertical   = null,
  }) {
    return .of(
      horizontal: horizontal,
      vertical: vertical,
      child: this,
    );
  }

}

// #endregion

// #region inherit

class MediaQueryInheritedArea extends StatelessWidget {

  const MediaQueryInheritedArea._({
    super.key,
    required this.data,
    required this.child,
  });

  const MediaQueryInheritedArea.of({
    Key?                    key,
    required MediaQueryData data,
    required Widget         child,
  }) : this._(
    key: key,
    data: data,
    child: child,
  );

  // ----------------

  final MediaQueryData data;
  final Widget         child;

  // ----------------

  @override
  build(context) {
    return MediaQuery(
      data: this.data,
      child: this.child,
    );
  }

}

// ----------------

class TextStyleInheritedArea extends StatelessWidget {

  const TextStyleInheritedArea._({
    super.key,
    required this.style,
    required this.align,
    required this.child,
  });

  const TextStyleInheritedArea.of({
    Key?               key,
    required TextStyle style,
    TextAlign?         align = null,
    required Widget    child,
  }) : this._(
    key: key,
    style: style,
    align: align,
    child: child,
  );

  // ----------------

  final TextStyle  style;
  final TextAlign? align;
  final Widget     child;

  // ----------------

  @override
  build(context) {
    return DefaultTextStyle(
      style: this.style,
      textAlign: this.align,
      child: this.child,
    );
  }

}

// #endregion

// #region advanced

class ListContainer extends StatelessWidget {

  const ListContainer._({
    super.key,
    required this.shrink,
    required this.padding,
    required this.controller,
    required this.itemCount,
    required this.itemBuilder,
  });

  const ListContainer.of({
    Key?                                                           key = null,
    Boolean                                                        shrink = false,
    EdgeInsetsGeometry?                                            padding = null,
    ScrollController?                                              controller = null,
    required Integer                                               itemCount,
    required Widget? Function(BuildContext context, Integer index) itemBuilder,
  }) : this._(
    key: key,
    shrink: shrink,
    padding: padding,
    controller: controller,
    itemCount: itemCount,
    itemBuilder: itemBuilder,
  );

  // ----------------

  final Boolean                                               shrink;
  final EdgeInsetsGeometry?                                   padding;
  final ScrollController?                                     controller;
  final Integer                                               itemCount;
  final Widget? Function(BuildContext context, Integer index) itemBuilder;

  // ----------------

  @override
  build(context) {
    return ListView.builder(
      shrinkWrap: this.shrink,
      physics: !this.shrink ? null : NeverScrollableScrollPhysics(),
      padding: this.padding,
      controller: this.controller,
      itemCount: this.itemCount,
      itemBuilder: this.itemBuilder,
    );
  }

}

// ----------------

class IconView extends StatelessWidget {

  const IconView._({
    super.key,
    required this.icon,
    required this.size,
    required this.fill,
    required this.color,
  });

  const IconView.of(
    IconData icon, {
    Key?     key = null,
    Floater? size = null,
    Floater? fill = null,
    Color?   color = null,
  }) : this._(
    key: key,
    icon: icon,
    size: size,
    fill: fill,
    color: color,
  );

  // ----------------

  final IconData icon;
  final Floater? size;
  final Floater? fill;
  final Color?   color;

  // ----------------

  @override
  build(context) {
    return Icon(
      this.icon,
      size: this.size,
      fill: this.fill,
      color: this.color,
    );
  }

}

class ImageView extends StatelessWidget {

  const ImageView._({
    super.key,
    required this.fit,
    required this.width,
    required this.height,
    required this.source,
  });

  const ImageView.of({
    Key?                   key = null,
    BoxFit?                fit = null,
    Floater?               width = null,
    Floater?               height = null,
    required ImageProvider source,
  }) : this._(
    key: key,
    fit: fit,
    width: width,
    height: height,
    source: source,
  );

  // ----------------

  final BoxFit?       fit;
  final Floater?      width;
  final Floater?      height;
  final ImageProvider source;

  // ----------------

  @override
  build(context) {
    return Image(
      fit: this.fit,
      width: this.width,
      height: this.height,
      image: this.source,
    );
  }

}

// #endregion
