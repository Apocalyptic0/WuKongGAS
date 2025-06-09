# GAS学习
## 初始化ASC
```C++
AbilitySystemComponent->InitAbilityActorInfo(Owner, Avatar);
```
两个参数分别是ASC的拥有者和作用对象，注意是ASC的而不是GA的

## 响应GameplayTag的改变
```C++
AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGDPlayerState::StunTagChanged);

virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
```
参数：
- 关注的Tag
- 添加还是删除
- 响应的委托

## 属性Attribute 
GAS中的属性为FGameplayAttributeData定义的浮点值，应该仅被GE修改 \
FGameplayAttributeData中有BaseValue和CurrentValue，BaseValue用于效果取消后属性的恢复
- 立即（Instant）和周期性（Periodic） GameplayEffects 将永久改变BaseValue
- 持续（Duration） 和永恒（Infinite） GameplayEffects 将改变CurrentValue。

### 响应Attribute的改变
```C++
AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGDPlayerState::HealthChanged);

virtual void HealthChanged(const FOnAttributeChangeData& Data);
```
FOnAttributeChangeData带有NewValue和OldValue

### 定义Attribute
```C++
// Uses macros from AttributeSet.h
// AttributeSetClass, PropertyName
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
```
这个宏可以自动生成Getter Setter Initter

PreAttributeChange
```C++
PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    if (Attribute == GetMoveSpeedAttribute())
    {
	    // Cannot slow less than 150 units/s and cannot boost more than 1000 units/s
	    NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
    }
}
```
- 任何Attributes的修改都会触发
- 仅修改查询了NewValue
- 不要用来处理游戏性事件，仅处理clamping

PostGameplayEffectExecute()
```C++
PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
```
在GE起作用后调用，可以进一步处理属性，也可以用来在服务器端clamping，该函数调用时属性虽然已经被GE修改，但并未返回客户端。

聚合器Aggregator \
GAS在AttributeSet中会自动创建聚合器，聚合器用来管理对同一属性的多个影响 \
在聚合器创建后会调用
```C++
virtual void OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute, FAggregator* NewAggregator) const override;
```
可以在这里修改聚合器的行为
```C++
void UGSAttributeSetBase::OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute, FAggregator* NewAggregator) const
{
	Super::OnAttributeAggregatorCreated(Attribute, NewAggregator);

	if (!NewAggregator)
	{
		return;
	}

	if (Attribute == GetMoveSpeedAttribute())
	{
        // 应用最低负数和所有正数修改
		NewAggregator->EvaluationMetaData = &FAggregatorEvaluateMetaDataLibrary::MostNegativeMod_AllPositiveMods;
	}
}
```

## GameplayEffect 
修改Attribute和GameplayTag，添加GameplayCue 
| 持续类型 | GameplayCue事件 | 使用情况 | 
|---|---|---|
| Instant | Execute | 立即永久改变Attribute的BaseValue，无法修改GameplayTags |
| Duration | Add & Remove | 临时修改Attribute的CurrentValue，添加GameplayTags，在过期后自动删除 |
| Infinite | Add & Remove | 临时修改Attribute的CurrentValue，添加GameplayTags，永不过时，通过GA或者ASC删除 |

周期性（Periodic Effects）效果算作Instant

### 添加GE
```C++
// 应用 ApplyGameplayEffectTo
UAbilitySystemComponent::ApplyGameplayEffectSpecToSelf();

// 监听
AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &APACharacterBase::OnActiveGameplayEffectAddedCallback);

// 回调
virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
```

### 移除GE
```C++
// 移出
FActiveGameplayEffectsContainer::RemoveActiveEffects();

// 监听
AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &APACharacterBase::OnRemoveGameplayEffectCallback);

// 回调
virtual void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved);
```

### GE修改器 Modifiers 
四种修改类型 Add Multiply Divide Override \
四种修改器 Scalable Float, Attribute Based, Custom Calculation Class, and Set By Caller 